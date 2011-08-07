#include "boolean_literal.h"

#include <sstream>

namespace ast
{
    boolean_literal::boolean_literal(chime::parser* parser)
    {
        chime::token* t;
        
        t = parser->next_token();
        assert(t->is_boolean());
        
        if (t->equal_to("true"))
            this->value(1);
        else if (t->equal_to("false"))
            this->value(0);
        else
            assert(0 && "Token should be a boolean literal!");
        
        delete t;
    }
    
    std::string boolean_literal::node_name(void) const
    {
        return std::string("boolean literal");
    }
    std::string boolean_literal::to_string(void) const
    {
        std::stringstream s;
        
        s << "literal: ";
        
        if (this->value())
            s << "true";
        else
            s << "false";
        
        return s.str();
    }
    
    unsigned char boolean_literal::value(void) const
    {
        return _value;
    }
    
    void boolean_literal::value(unsigned char v)
    {
        _value = v;
    }
    
    llvm::Value* boolean_literal::codegen(chime::code_generator& generator)
    {
        return generator.call_chime_literal_encode_boolean(this->value());
    }
}
