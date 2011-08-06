#ifndef AST_ROOT_H
#define AST_ROOT_H

#include "node.h"

#include <vector>
#include <string>
#include <tr1/memory>

namespace ast
{
    class Root : public node
    {
    public:
        Root();
        virtual ~Root();
        
        std::vector<std::string> getBinaryDependencies(void) const;
        void                     addBinaryDependency(const std::string& dependency);
        
        std::vector<std::string> getSourceDependencies(void) const;
        void                     addSourceDependency(const std::string& dependency);
    
    private:
        std::vector<std::string> _binaryDependencies;
        std::vector<std::string> _sourceDependencies;
    };
    
    typedef std::tr1::shared_ptr<Root> RootRef;
}

#endif // AST_ROOT_H
