// implementation.h

#ifndef IMPLEMENTATION_NODE_H
#define IMPLEMENTATION_NODE_H

#include "compiler/ast/node.h"
#include "compiler/ast/primary/type_reference.h"
#include "compiler/ast/common/code_block.h"

#include <llvm/Function.h>

namespace ast
{
    class Implementation : public node
    {
    public:
        Implementation(chime::parser& parser);
        virtual ~Implementation();
        
        virtual std::string nodeName(void) const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        llvm::Function*     createInitFunction(chime::code_generator& generator);
        llvm::Value*        codegen(chime::code_generator& generator);
        
        TypeRef    getTypeRef() const;
        void       setTypeRef(ast::TypeRef node);
        TypeRef    getSuperclass() const;
        void       setSuperclass(ast::TypeRef node);
        CodeBlock* getBody(void) const;
        
    protected:
        TypeRef    _typeRef;
        TypeRef    _superclass;
        CodeBlock* _bodyBlock;
    };
}

#endif // IMPLEMENTATION_NODE_H
