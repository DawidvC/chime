#ifndef BOOLEAN_LITERAL
#define BOOLEAN_LITERAL

#include "Literal.h"

namespace ast
{
    class boolean_literal : public Literal
    {
    public:
        boolean_literal(chime::parser* parser);
        
        virtual std::string node_name(void) const;
        virtual std::string to_string(void) const;
        
        unsigned char value(void) const;
        void          value(unsigned char v);
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
    
    protected:
        unsigned char _value;
    };
}

#endif
