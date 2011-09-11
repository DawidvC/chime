// implementation.h

#ifndef IMPLEMENTATION_NODE_H
#define IMPLEMENTATION_NODE_H

#include "compiler/ast/ScopedNode.h"
#include "compiler/ast/primary/type_reference.h"
#include "compiler/ast/common/code_block.h"

#include <llvm/Function.h>

namespace ast
{
    class Implementation : public ScopedNode
    {
    public:
        static Implementation* parse(chime::parser& parser);
    
    public:
        virtual std::string nodeName(void) const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        TypeRef    getTypeRef() const;
        void       setTypeRef(ast::TypeRef node);
        TypeRef    getSuperclass() const;
        void       setSuperclass(ast::TypeRef node);
        NodeRef    getBody(void) const;
        
        std::string         getIdentifier() const;
        
        Variable*           createVariable(const std::string& identifier);
        
        llvm::Function*     createInitFunction(chime::code_generator& generator);
        llvm::Value*        codegen(chime::code_generator& generator);
        
    protected:
        TypeRef _typeRef;
        TypeRef _superclass;
        NodeRef _bodyBlock;
    };
}

#endif // IMPLEMENTATION_NODE_H
