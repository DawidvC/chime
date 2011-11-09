// chime: FloatLiteral.h

#ifndef FLOAT_LITERAL_H
#define FLOAT_LITERAL_H

#include "Literal.h"

namespace chime
{
    class FloatLiteral : public ast::Literal
    {
    public:
        FloatLiteral();
        
        virtual std::string nodeName() const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        double getValue() const;
        void setValue(double v);
        
        virtual llvm::Value* codegen(CodeGenContext& generator);
        
    private:
        double _value;
    };
}

#endif // FLOAT_LITERAL_H
