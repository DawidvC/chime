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
    std::string string_literal::stringRepresentation(int depth) const
    {
        std::string s;
        
        s.append(depth*2, ' ');
        s.append("literal: '");
        s.append(this->value());
        s.append("'");
        
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
        llvm::Value* cStringPtr;
        llvm::Value* lengthValue;
        
        cStringPtr  = generator.getConstantString(this->value());
        lengthValue = llvm::ConstantInt::get(generator.getContext(), llvm::APInt(64, this->value().length(), 10));
        
        return generator.getRuntime()->callChimeStringCreateWithCString(cStringPtr, lengthValue);
    }
}
