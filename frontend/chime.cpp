#include "compiler/compiler.h"

#include <cstdio>
#include <string>
#include <getopt.h>
#include <unistd.h>
#include <sys/stat.h>

#include <llvm/Pass.h>
#include <llvm/PassManager.h>
#include <llvm/Assembly/PrintModulePass.h>
#include <llvm/Support/FormattedStream.h>

bool                              print_ast;
bool                              emit_llvm_ir;
bool                              _executeBinary;
bool                              _traceSteps;
bool                              _buildOptimized;
std::string                       _inputFileName;
std::string                       _outputFileName;
std::vector<chime::SourceFileRef> _compiledSources;
std::vector<std::string>          _binaryDependencies;

void get_options(int argc, char* argv[])
{
    int32_t c;
    
    static struct option longopts[] = {
        { "compile",   no_argument,       NULL, 'c' },
        { "debug",     no_argument,       NULL, 'd' },
        { "emit-llvm", no_argument,       NULL, 'e' },
        { "help",      no_argument,       NULL, 'h' },
        { "output",    required_argument, NULL, 'o' },
        { "print",     no_argument,       NULL, 'p' },
        { "trace",     no_argument,       NULL, 't' },
        { NULL,        0,                 NULL, 0   }
    };
    
    print_ast       = false;
    emit_llvm_ir    = false;
    _executeBinary  = true;
    _traceSteps     = false;
    _buildOptimized = true;
    
    while ((c = getopt_long(argc, argv, "cdeho:pt", longopts, NULL)) != -1)
    {
        switch (c)
        {
            case 'c':
                _executeBinary = false;
                break;
            case 'd':
                _buildOptimized = false;
                break;
            case 'e':
                emit_llvm_ir = true;
                break;
            case 'o':
                _outputFileName = std::string(optarg);
                break;
            case 'p':
                print_ast = true;
                break;
            case 't':
                _traceSteps = true;
                break;
            case 'h':
            default:
                printf("usage: chime [options] [--] [file] [arguments]\n");
                printf("\n");
                printf("  -c                run only compile and assemble steps\n");
                printf("  -d (--debug)      disable optimizations\n");
                printf("  -e (--emit-llvm)  create llvm IR\n");
                printf("  -h (--help)       print this help message and exit\n");
                printf("  -o (--output)     name of output file\n");
                printf("  -p (--print)      print out the AST representation for the input file\n");
                printf("  -t (--trace)      trace compilation steps\n");
                exit(0);
                break;
        }
    }
}

void produce_ir(llvm::Module& module)
{
    llvm::PassManager  passManager;
    llvm::raw_ostream* stream;
    
    if (!_outputFileName.empty())
    {
        std::string errorString;
        
        stream = new llvm::raw_fd_ostream(_outputFileName.c_str(), errorString, 0);
    }
    else
    {
        stream = &llvm::outs();
    }
    
    passManager.add(llvm::createPrintModulePass(stream));
    
    passManager.run(module);
}

std::string resolvedPathForDependency(std::string name)
{
    // passthrough for inputFileName
    if (name.compare(_inputFileName) == 0)
        return name;
    
    // a leading slash means it is not relative
    if (*name.begin() == '/')
        return name + ".chm";
    
    return _inputFileName.substr(0, _inputFileName.find_last_of("/") + 1) + name + ".chm";
}

bool compile(chime::SourceFileRef sourceFile, bool asMain)
{
    std::vector<std::string>::iterator i;
    std::vector<std::string>           dependencies;
    std::string                        sourcePath;
    struct stat                        results;
    
    if (_traceSteps)
        fprintf(stdout, "[Compile] %s\n", sourceFile->getPath().c_str());
    
    if (!sourceFile->writeObjectFile(asMain, _buildOptimized))
    {
        fprintf(stderr, "[Compile] failed\n");
        
        return false;
    }
    
    _compiledSources.push_back(sourceFile);
    
    // first, accumulate all the binary dependencies
    dependencies = sourceFile->getBinaryDependencies();
    
    _binaryDependencies.insert(_binaryDependencies.end(), dependencies.begin(), dependencies.end());
    
    // now, compile all the source dependencies
    dependencies = sourceFile->getSourceDependencies();
    
    for (i = dependencies.begin(); i < dependencies.end(); ++i)
    {
        chime::SourceFileRef dependencySource;
        std::string          dependencyPath;
        
        dependencyPath = resolvedPathForDependency((*i));
        
        if (lstat(dependencyPath.c_str(), &results) != 0)
        {
            fprintf(stderr, "[Compile] Dependency '%s' could not be resolved\n", dependencyPath.c_str());
            return false;
        }
        
        dependencySource = chime::SourceFileRef(new chime::SourceFile(dependencyPath));
        
        if (!compile(dependencySource, false))
        {
            fprintf(stderr, "[Compile] Sub compilation failed for %s\n", dependencySource->getPath().c_str());
            
            return false;
        }
    }
    
    return true;
}

bool link(void)
{
    std::string linkCommand;
    struct stat results;
    
    linkCommand = std::string("/usr/bin/ld");
    
    if (lstat(linkCommand.c_str(), &results) != 0)
    {
        fprintf(stderr, "[Link] Could not access the linker command '%s'\n", linkCommand.c_str());
        return false;
    }
    
    linkCommand.append(" -dynamic -arch x86_64 -macosx_version_min 10.6.0 -lcrt1.10.6.o -lSystem");
    linkCommand.append(" -lchimeruntime -lchime");
    
    if (_compiledSources.size() == 0)
    {
        fprintf(stderr, "[Link] Zero compiled sources to link\n");
        return false;
    }
    
    std::vector<chime::SourceFileRef>::iterator i;
    for (i = _compiledSources.begin(); i < _compiledSources.end(); ++i)
    {
        linkCommand.append(" ");
        linkCommand.append((*i)->getOutputFilePath());
    }
    
    if (!_outputFileName.empty())
    {
        linkCommand.append(" -o ");
        linkCommand.append(_outputFileName);
    }
    
    if (_traceSteps)
        fprintf(stdout, "[Link] %s\n", _outputFileName.c_str());
        
    if (system(linkCommand.c_str()) != 0)
    {
        fprintf(stderr, "[Link] failed\n");
        fprintf(stderr, "[Link] command: %s\n", linkCommand.c_str());
        
        return false;
    }
    
    return true;
}

int main(int argc, char* argv[])
{
    chime::SourceFileRef sourceFile;
    struct stat          results;
    
    get_options(argc, argv);
    argc -= optind;
    argv += optind;
    
    if (argc == 0)
    {
        // no input files, should go to interactive mode here
        fprintf(stderr, "[Compile] No input files\n");
        return 1;
    }
    
    if (lstat(argv[0], &results) != 0)
    {
        fprintf(stderr, "[Compile] Could not access the file '%s'\n", argv[0]);
        return 1;
    }
    
    _inputFileName = std::string(argv[0]);
    
    sourceFile = chime::SourceFileRef(new chime::SourceFile(_inputFileName));
    
    if (print_ast)
    {
        fprintf(stdout, "%s\n", sourceFile->getASTRoot()->stringRepresentation().c_str());
        
        return 0;
    }
    
    if (emit_llvm_ir)
    {
        produce_ir(*sourceFile->getModule(true));
        return 0;
    }
    
    // now actually begin the full compilation process
    if (!compile(sourceFile, true))
        return 1;
    
    // set a default path if needed
    if (_outputFileName.empty())
    {
        _outputFileName = sourceFile->getBinaryFilePath();
    }
    
    if (!link())
        return 1;
    
    if (_executeBinary)
    {
        return execl(_outputFileName.c_str(), NULL);
    }
    
    return 0;
}
