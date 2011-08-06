#ifndef SOURCE_FILE_H
#define SOURCE_FILE_H

#include "ast/Root.h"

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
        
        std::string   getPath(void) const;
        std::string   getModuleName(void) const;
        std::string   getOutputFilePath(void) const;
        
        ast::RootRef  getASTRoot(void);
        llvm::Module* getModule(bool asMain);
        
        bool          writeObjectFile(bool asMain);
        
        std::vector<std::string> getSourceDependencies(void);
        std::vector<std::string> getBinaryDependencies(void);
        
    private:
        std::string  _path;
        ast::RootRef _astRoot;
    };
    
    typedef std::tr1::shared_ptr<SourceFile> SourceFileRef;
}

#endif // SOURCE_FILE_H