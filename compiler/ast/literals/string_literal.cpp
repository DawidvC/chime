#include "string_literal.h"

namespace ast
{
    string_literal::string_literal(chime::parser* parser)
    {
        chime::token* t;
        
        assert(parser->look_ahead()->is_string());
        
        t = parser->next_token();
        this->value(t->string_value());
        
        delete t;
    }
    
    std::string string_literal::nodeName(void) const
    {
        return std::string("string literal");
    }
    std::string string_literal::toString(void) const
    {
        std::string s;
        
        s += "literal: '" + this->value() + "'";
        
        return s;
    }
    
    std::string string_literal::value(void) const
    {
        return _value;
    }
    void string_literal::value(std::string v)
    {
        _value = v;
    }
    
    llvm::Value* string_literal::codegen(chime::code_generator& generator)
    {
        return generator.call_chime_string_create_with_c_string(this->value());
    }
}