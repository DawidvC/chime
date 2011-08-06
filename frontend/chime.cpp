#include <cstdio>
#include <string>
#include <getopt.h>
#include <libgen.h>

#include "lexer/file_lexer.h"
#include "parser/parser.h"
#include "codegen/code_generator.h"

#include <llvm/Pass.h>
#include <llvm/PassManager.h>
#include <llvm/Assembly/PrintModulePass.h>
#include <llvm/Support/FormattedStream.h>

// for emitting object files
#include "llvm/Support/Host.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetRegistry.h"
#include "llvm/Target/TargetSelect.h"

bool        print_ast;
bool        emit_llvm_ir;
const char* outputFileName;
bool        compileAsMain;

// std::string path_with_new_extension(std::string path, const char* extension)
// {
//     path = path.substr(0, path.find_last_of("."));
//     
//     return path + extension;
// }

void get_options(int argc, char* argv[])
{
    int32_t c;
    
    static struct option longopts[] = {
        { "compile",   no_argument,       NULL, 'c' },
        { "emit-llvm", no_argument,       NULL, 'e' },
        { "help",      no_argument,       NULL, 'h' },
        { "main",      no_argument,       NULL, 'm' },
        { "output",    required_argument, NULL, 'o' },
        { "print",     no_argument,       NULL, 'p' },
        { NULL,        0,                 NULL, 0   }
    };
    
    print_ast      = false;
    emit_llvm_ir   = false;
    outputFileName = NULL;
    compileAsMain  = false;
    
    while ((c = getopt_long(argc, argv, "cehmo:p", longopts, NULL)) != -1)
    {
        switch (c)
        {
            case 'c':
                break;
            case 'e':
                emit_llvm_ir = true;
                break;
            case 'm':
                compileAsMain = true;
                break;
            case 'o':
                outputFileName = optarg;
                break;
            case 'p':
                print_ast = true;
                break;
            case 'h':
            default:
                printf("usage: chime [options] [--] [file] [arguments]\n");
                printf("\n");
                printf("  -c                run only compile and assemble steps\n");
                printf("  -e (--emit-llvm)  create llvm IR\n");
                printf("  -h (--help)       print this help message and exit\n");
                printf("  -m (--main)       compile for program start\n");
                printf("  -o (--output)     name of output file\n");
                printf("  -p (--print)      print out the AST representation for the input file\n");
                exit(0);
                break;
        }
    }
}

void produce_ir(llvm::Module& module)
{
    llvm::PassManager  passManager;
    llvm::raw_ostream* stream;
    
    if (outputFileName)
    {
        std::string errorString;
        
        stream = new llvm::raw_fd_ostream(outputFileName, errorString, 0);
    }
    else
    {
        stream = &llvm::outs();
    }
    
    passManager.add(llvm::createPrintModulePass(stream));
    
    passManager.run(module);
}

void produce_object_file(llvm::Module& module)
{
    llvm::PassManager    passManager;
    std::string          errorString;
    llvm::Triple         triple;
    const llvm::Target*  target = 0;
    llvm::TargetMachine* targetMachine;
    std::string          featuresString;
    llvm::raw_ostream*   stream;
    
    stream = new llvm::raw_fd_ostream(outputFileName, errorString, llvm::raw_fd_ostream::F_Binary);
    
    llvm::formatted_raw_ostream formatted_ostream(*stream);
    
    // initialize the target and asm generator for just this machine
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    
    triple.setTriple(llvm::sys::getHostTriple());
    
    target = llvm::TargetRegistry::lookupTarget(triple.getTriple(), errorString);
    
    if (!target)
    {
        fprintf(stderr, "crap %s\n", errorString.c_str());
    }
    
    targetMachine = target->createTargetMachine(triple.getTriple(), featuresString);
    
    passManager.add(new llvm::TargetData((const llvm::TargetData)*targetMachine->getTargetData()));
    
    targetMachine->setAsmVerbosityDefault(true);
    
    // llvm::TargetMachine::CGFT_ObjectFile
    if (targetMachine->addPassesToEmitFile(passManager, formatted_ostream, llvm::TargetMachine::CGFT_ObjectFile, llvm::CodeGenOpt::None, false))
    {
        fprintf(stderr, "Target type does not support object file generation\n");
    }
    
    passManager.run(module);
}

int main(int argc, char* argv[])
{
    chime::parser*         parser;
    chime::filelexer*      lexer;
    ast::node*             node;
    chime::code_generator* generator;
    
    get_options(argc, argv);
    argc -= optind;
    argv += optind;
    
    lexer     = new chime::filelexer(argv[0]);
    parser    = new chime::parser(lexer);
    generator = new chime::code_generator();
    
    node = parser->parse();
    if (!node)
        return 1;
    
    parser->print_errors();
    
    if (print_ast)
    {
        fprintf(stdout, "%s\n", node->stringRepresentation().c_str());
        
        return 0;
    }
    
    generator->generate(node, std::string(basename((char *)outputFileName)), compileAsMain);
    
    if (emit_llvm_ir)
    {
        produce_ir(*generator->module());
        return 0;
    }
    
    produce_object_file(*generator->module());
    
    return 0;
}
