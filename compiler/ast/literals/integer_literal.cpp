#include "integer_literal.h"

#include <sstream>

namespace chime
{
    IntegerLiteral::IntegerLiteral()
    {
        _value = 0;
    }
    
    std::string IntegerLiteral::nodeName(void) const
    {
        return "Integer Literal";
    }
    std::string IntegerLiteral::stringRepresentation(int depth) const
    {
        std::stringstream s;
        
        s << std::string("").append(depth*2, ' ');
        s << this->nodeName();
        s << ": ";
        s << this->getValue();
        
        return s.str();
    }
    
    long IntegerLiteral::getValue() const
    {
        return _value;
    }
    void IntegerLiteral::setValue(long v)
    {
        _value = v;
    }
    
    llvm::Value* IntegerLiteral::codegen(chime::code_generator& generator)
    {
        llvm::Value* integerValue;
        
        integerValue = llvm::ConstantInt::get(generator.getContext(), llvm::APInt(64, this->getValue(), 10));
        
        return generator.getRuntime()->callChimeLiteralEncodeInteger(integerValue);
    }
}
