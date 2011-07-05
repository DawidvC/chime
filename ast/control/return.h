#ifndef RETURN_H
#define RETURN_H

#include "ast/node.h"

namespace ast
{
    class Return : public node
    {
    public:
        Return(chime::parser& parser);
        
        virtual std::string nodeName(void) const;
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
    };
}

#endif // RETURN_H
