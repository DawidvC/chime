#ifndef SOURCE_FILE_H
#define SOURCE_FILE_H

#include "compiler/ast/Root.h"

#include <string>
#include <vector>
#include <tr1/memory>

#include <llvm/Module.h>

namespace chime
{
    class SourceFile
    {
    public:
        static std::string pathWithNewExtension(std::string path, const char* extension);
    
    public:
        SourceFile(const std::string& path);
        
        std::string   getPath() const;
        std::string   getModuleName() const;
        std::string   getOutputFilePath(void) const;
        std::string   getBinaryFilePath() const;
        
        RootRef       getASTRoot();
        llvm::Module* getModule(bool asMain);
        
        bool          writeObjectFile(bool asMain, bool optimized);
        
        std::vector<std::string> getSourceDependencies();
        std::vector<std::string> getBinaryDependencies();
        
    private:
        std::string _path;
        RootRef     _astRoot;
    };
    
    typedef std::tr1::shared_ptr<SourceFile> SourceFileRef;
}

#endif // SOURCE_FILE_H