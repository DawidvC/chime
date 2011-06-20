// implementation.h

#ifndef IMPLEMENTATION_NODE_H
#define IMPLEMENTATION_NODE_H

#include "ast/node.h"
#include "ast/primary/type_reference.h"
#include "ast/common/code_block.h"

namespace ast
{
    class Implementation : public node
    {
    public:
        Implementation(chime::parser& parser);
        virtual ~Implementation();
        
        virtual std::string nodeName(void) const;
        virtual std::string to_string(void) const;
        
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
