#ifndef SELF_LITERAL_H
#define SELF_LITERAL_H

#include "Literal.h"

namespace ast
{
    class SelfLiteral : public Literal
    {
    public:
        SelfLiteral(chime::parser& parser);
        
        virtual std::string nodeName(void) const;
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
    };
}

#endif // SELF_LITERAL_H
