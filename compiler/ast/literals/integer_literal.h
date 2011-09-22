// integer_literal.h

#ifndef INTEGER_LITERAL_H
#define INTEGER_LITERAL_H

#include "Literal.h"

namespace ast
{
    class integer_literal : public Literal
    {
    public:
        integer_literal();
        integer_literal(chime::parser* parser);
        
        virtual std::string nodeName(void) const;
        virtual std::string to_string(void) const;
        
        long value(void) const;
        void value(long v);
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
        
    private:
        long _value;
    };
}

#endif // INTEGER_LITERAL_H
