#include "FloatLiteral.h"

#include <sstream>

namespace chime
{
    FloatLiteral::FloatLiteral()
    {
        _value = 0.0;
    }
    
    std::string FloatLiteral::nodeName(void) const
    {
        return "Float Literal";
    }
    std::string FloatLiteral::stringRepresentation(int depth) const
    {
        std::stringstream s;
        
        s << std::string("").append(depth*2, ' ');
        s << this->nodeName();
        s << ": ";
        s << this->getValue();
        
        return s.str();
    }
    
    double FloatLiteral::getValue() const
    {
        return _value;
    }
    void FloatLiteral::setValue(double v)
    {
        _value = v;
    }
    
    llvm::Value* FloatLiteral::codegen(CodeGenContext& context)
    {
        llvm::Value* doubleValue;
        
        doubleValue = llvm::ConstantFP::get(context.getContext(), llvm::APFloat(this->getValue()));
        
        return context.getRuntime()->callChimeFloatCreate(doubleValue);
    }
}
