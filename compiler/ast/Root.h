// chime: ScopedNode.h

#ifndef AST_ROOT_H
#define AST_ROOT_H

#include "ScopedNode.h"

namespace ast
{
    class Root : public ScopedNode
    {
    public:
        std::vector<std::string> getBinaryDependencies(void) const;
        void                     addBinaryDependency(const std::string& dependency);
        
        std::vector<std::string> getSourceDependencies(void) const;
        void                     addSourceDependency(const std::string& dependency);
        
        std::string              getIdentifier() const;
        
        Variable*                createVariable(const std::string& identifier);
        bool                     allowsStructuralElements() const;
        
    private:
        std::vector<std::string> _binaryDependencies;
        std::vector<std::string> _sourceDependencies;
    };
    
    typedef std::tr1::shared_ptr<Root> RootRef;
}

#endif // AST_ROOT_H
