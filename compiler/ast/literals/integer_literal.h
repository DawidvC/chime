// integer_literal.h

#ifndef INTEGER_LITERAL
#define INTEGER_LITERAL

#include "compiler/ast/node.h"

namespace ast
{
    class integer_literal : public node
    {
    public:
        integer_literal();
        integer_literal(chime::parser* parser);
        
        virtual std::string node_name(void) const;
        virtual std::string to_string(void) const;
        
        long value(void) const;
        void value(long v);
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
        
    private:
        long _value;
    };
}

#endif