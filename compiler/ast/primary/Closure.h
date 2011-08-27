#ifndef CLOSURE_H
#define CLOSURE_H

#include "compiler/ast/node.h"
#include "compiler/ast/common/parameter_set.h"

namespace ast
{
    class Closure : public node
    {
    public:
        Closure(chime::parser& parser);
        
        virtual std::string nodeName(void) const;
        NodeRef             getBody() const;
        ParameterSetRef     getParameters() const;
        
        llvm::Value* codegen(chime::code_generator& generator);
    
    private:
        NodeRef         _bodyBlock;
        ParameterSetRef _parameters;
    };
}

#endif
