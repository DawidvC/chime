#ifndef STRING_LITERAL_H
#define STRING_LITERAL_H

#include "Literal.h"

namespace ast
{
    class string_literal : public Literal
    {
    public:
        string_literal(chime::parser* parser);
        
        virtual std::string nodeName(void) const;
        virtual std::string toString(void) const;
        
        std::string value(void) const;
        void value(std::string v);
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
        
    private:
        std::string _value;
    };
}

#endif // STRING_LITERAL_H
