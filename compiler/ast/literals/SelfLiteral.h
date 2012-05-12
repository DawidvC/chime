// chime:: SelfLiteral.h

#ifndef SELF_LITERAL_H
#define SELF_LITERAL_H

#include "Literal.h"

namespace chime
{
    class SelfLiteral : public Literal
    {
    public:
        SelfLiteral();
        
        virtual std::string nodeName() const;
        
        bool isSelf() const;
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
    };
}

#endif // SELF_LITERAL_H
