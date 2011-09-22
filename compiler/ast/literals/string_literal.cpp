#include "string_literal.h"

namespace ast
{
    ast::Node* string_literal::parse(chime::parser& parser)
    {
        ast::string_literal* string;
        
        string = new ast::string_literal();
        string->value(parser.next_token()->string_value());
        
        return string;
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
