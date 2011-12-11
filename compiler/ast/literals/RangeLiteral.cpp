#include "RangeLiteral.h"

#include <sstream>

namespace chime
{
    RangeLiteral::RangeLiteral()
    {
        _startValue = 0;
        _endValue   = 0;
    }
    
    std::string RangeLiteral::nodeName() const
    {
        return "Range Literal";
    }
    
    std::string RangeLiteral::stringRepresentation(int depth) const
    {
        std::stringstream s;
        
        s << std::string("").append(depth*2, ' ');
        s << this->nodeName();
        s << ": ";
        s << this->getStartValue();
        s << ":";
        s << this->getEndValue();
        
        return s.str();
    }
    
    long RangeLiteral::getStartValue() const
    {
        return _startValue;
    }
    
    void RangeLiteral::setStartValue(long value)
    {
        _startValue = value;
    }
    
    long RangeLiteral::getEndValue() const
    {
        return _endValue;
    }
    
    void RangeLiteral::setEndValue(long value)
    {
        _endValue = value;
    }
    
    llvm::Value* RangeLiteral::codegen(CodeGenContext& context)
    {
        llvm::Value* startValue;
        llvm::Value* endValue;
        
        startValue = llvm::ConstantInt::get(context.getContext(), llvm::APInt(64, this->getStartValue(), 10));
        endValue   = llvm::ConstantInt::get(context.getContext(), llvm::APInt(64, this->getEndValue(), 10));
        
        startValue = context.getRuntime()->callChimeIntegerEncode(startValue);
        endValue   = context.getRuntime()->callChimeIntegerEncode(endValue);
        
        return context.getRuntime()->callChimeRangeCreate(startValue, endValue);
    }
}
