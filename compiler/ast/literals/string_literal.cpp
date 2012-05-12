#include "string_literal.h"

namespace chime
{
    Node* string_literal::parse(chime::parser& parser)
    {
        string_literal* string;
        
        string = new string_literal();
        string->value(parser.next_token()->stringValue());
        
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
        llvm::Value* stringValue;
        
        cStringPtr  = generator.getConstantString(this->value());
        lengthValue = llvm::ConstantInt::get(generator.getContext(), llvm::APInt(64, this->value().length(), 10));
        
        stringValue = generator.getRuntime()->callChimeStringCreateWithCString(cStringPtr, lengthValue);
        
        generator.getCurrentScope()->addLooseValue(stringValue);
        
        return stringValue;
    }
}
