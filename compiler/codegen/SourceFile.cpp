#include "SourceFile.h"
#include "compiler/parser/parser.h"
#include "compiler/codegen/code_generator.h"
#include "compiler/lexer/file_lexer.h"

#include <libgen.h>

// for emitting object files
#include <llvm/Pass.h>
#include <llvm/PassManager.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/Support/Host.h>
#include <llvm/ADT/Triple.h>
#include <llvm/Target/TargetData.h>
#include <llvm/Target/TargetMachine.h>
#include "llvm/Support/TargetRegistry.h"
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>

namespace chime
{
    std::string SourceFile::pathWithNewExtension(std::string path, const char* extension)
    {
        path = path.substr(0, path.find_last_of("."));
        
        return path + extension;
    }
    
    SourceFile::SourceFile(const std::string& path)
    {
        _path = path;
    }
    
    std::string SourceFile::getPath() const
    {
        return _path;
    }
    
    std::string SourceFile::getModuleName() const
    {
        size_t lastSlashPos;
        size_t lastPeriodPos;
        
        lastSlashPos  = _path.find_last_of("/");
        lastPeriodPos = _path.find_last_of(".");
        
        if (lastSlashPos > 0)
            lastSlashPos += 1;
        
        return _path.substr(lastSlashPos, lastPeriodPos-lastSlashPos);
    }
    
    std::string SourceFile::getOutputFilePath() const
    {
        return SourceFile::pathWithNewExtension(_path, ".chc");
    }
    
    std::string SourceFile::getBinaryFilePath() const
    {
        return SourceFile::pathWithNewExtension(_path, "");
    }
    
    RootRef SourceFile::getASTRoot()
    {
        if (!_astRoot)
        {
            chime::ParserRef    parser;
            chime::FileLexerRef lexer;
            
            lexer  = chime::FileLexerRef(new chime::filelexer(_path.c_str()));
            parser = chime::ParserRef(new chime::parser(lexer.get()));
            
            _astRoot = RootRef(parser->parse());
            
            parser->printErrors();
        }
        
        return _astRoot;
    }
    
    llvm::Module* SourceFile::getModule(bool asMain)
    {
        chime::code_generator* generator;
        
        generator = new chime::code_generator();
        
        // Right now, if you delete the generator, the module is deleted too
        // If we passed in a module, we could fix that
        
        // module = new llvm::Module(moduleName, llvm::getGlobalContext());
        
        generator->generate(this->getASTRoot().get(), this->getModuleName(), asMain);
        
        return generator->module();
    }
    
    bool SourceFile::writeObjectFile(bool asMain, bool optimized)
    {
        llvm::Module*           module;
        llvm::PassManager       passManager;
        std::string             errorString;
        llvm::Triple            triple;
        const llvm::Target*     target = 0;
        llvm::TargetMachine*    targetMachine;
        std::string             featuresString;
        llvm::raw_ostream*      stream;
        llvm::CodeGenOpt::Level optimizationLevel;
        
        module = this->getModule(asMain);
        
        assert(module);
        
        stream = new llvm::raw_fd_ostream(this->getOutputFilePath().c_str(), errorString, llvm::raw_fd_ostream::F_Binary);
        
        llvm::formatted_raw_ostream formatted_ostream(*stream);
        
        // initialize the target and asm generator for just this machine
        llvm::InitializeNativeTarget();
        llvm::InitializeNativeTargetAsmPrinter();
        
        triple.setTriple(llvm::sys::getHostTriple());
        
        target = llvm::TargetRegistry::lookupTarget(triple.getTriple(), errorString);
        if (!target)
        {
            fprintf(stderr, "Unable to create target from triple (%s)\n", errorString.c_str());
            return false;
        }
        
        targetMachine = target->createTargetMachine(triple.getTriple(), "", featuresString);
        if (!targetMachine)
        {
            fprintf(stderr, "Unable to create target machine for code generation\n");
            return false;
        }
        
        passManager.add(new llvm::TargetData((const llvm::TargetData)*targetMachine->getTargetData()));
        
        targetMachine->setAsmVerbosityDefault(true);
        
        if (optimized)
            optimizationLevel = llvm::CodeGenOpt::Aggressive;
        else
            optimizationLevel = llvm::CodeGenOpt::None;
        
        // llvm::TargetMachine::CGFT_ObjectFile
        if (targetMachine->addPassesToEmitFile(passManager, formatted_ostream, llvm::TargetMachine::CGFT_ObjectFile, optimizationLevel, false))
        {
            fprintf(stderr, "Target type does not support object file generation\n");
            return false;
        }
        
        passManager.run(*module);
        
        return true;
    }
    
    std::vector<std::string> SourceFile::getSourceDependencies()
    {
        return this->getASTRoot()->getSourceDependencies();
    }
    
    std::vector<std::string> SourceFile::getBinaryDependencies()
    {
        return this->getASTRoot()->getBinaryDependencies();
    }
}
