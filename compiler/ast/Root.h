// chime: ScopedNode.h

#ifndef AST_ROOT_H
#define AST_ROOT_H

#include "ScopedNode.h"

namespace ast
{
    class Root : public ScopedNode
    {
    public:
        Root();
        
        std::string nodeName() const;
        
        std::vector<std::string> getBinaryDependencies(void) const;
        void                     addBinaryDependency(const std::string& dependency);
        
        std::vector<std::string> getSourceDependencies(void) const;
        void                     addSourceDependency(const std::string& dependency);
        
        std::string              getIdentifier() const;
        void                     setIdentifier(const std::string& identifier);
        
        chime::Variable*         createVariable(const std::string& identifier);
        chime::SelfLiteral*      createSelf();
        bool                     allowsStructuralElements() const;
        
        llvm::Value*             codegen(chime::CodeGenContext& context);
        
    private:
        std::string              _identifier;
        std::vector<std::string> _binaryDependencies;
        std::vector<std::string> _sourceDependencies;
    };
    
    typedef std::tr1::shared_ptr<Root> RootRef;
}

#endif // AST_ROOT_H
