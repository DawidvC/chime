#include "integer_literal.h"

#include <sstream>

namespace ast
{
    integer_literal::integer_literal()
    {
        this->value(0);
    }
    
    integer_literal::integer_literal(chime::parser* parser)
    {
        chime::token* t;
        
        assert(parser->look_ahead()->is_integer());
        
        t = parser->next_token();
        this->value(t->integer_value());
        
        delete t;
    }
    
    std::string integer_literal::nodeName(void) const
    {
        return std::string("Integer Literal");
    }
    std::string integer_literal::to_string(void) const
    {
        std::stringstream s;
        
        s << "literal: ";
        s << this->value();
        
        return s.str();
    }
    
    long integer_literal::value(void) const
    {
        return _value;
    }
    void integer_literal::value(long v)
    {
        _value = v;
    }
    
    llvm::Value* integer_literal::codegen(chime::code_generator& generator)
    {
        llvm::Value* integerValue;
        
        integerValue = llvm::ConstantInt::get(generator.getContext(), llvm::APInt(64, this->value(), 10));
        
        return generator.getRuntime()->callChimeLiteralEncodeInteger(integerValue);
    }
}
