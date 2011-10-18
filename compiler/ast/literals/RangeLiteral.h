// chime: RangeLiteral.h

#ifndef RANGE_LITERAL_H
#define RANGE_LITERAL_H

#include "Literal.h"

namespace chime
{
    class RangeLiteral : public ast::Literal
    {
    public:
        RangeLiteral();
        
        virtual std::string nodeName() const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        long getStartValue() const;
        void setStartValue(long value);
        long getEndValue() const;
        void setEndValue(long value);
        
        llvm::Value* codegen(CodeGenContext& context);
    
    protected:
        long _startValue;
        long _endValue;
    };
}

#endif
