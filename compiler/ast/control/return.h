#ifndef RETURN_H
#define RETURN_H

#include "compiler/ast/node.h"

namespace ast
{
    class Return : public node
    {
    public:
        Return(chime::parser& parser);
        
        virtual std::string nodeName(void) const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        ast::NodeRef getReturnValue() const;
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
    
    private:
        ast::NodeRef _returnValue;
    };
}

#endif // RETURN_H
