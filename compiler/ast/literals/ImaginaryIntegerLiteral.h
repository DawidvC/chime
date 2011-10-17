// chime: ImaginaryIntegerLiteral.h

#ifndef IMAGINARY_INTEGER_LITERAL_H
#define IMAGINARY_INTEGER_LITERAL_H

#include "integer_literal.h"

namespace chime
{
    class ImaginaryIntegerLiteral : public IntegerLiteral
    {
    public:
        virtual std::string nodeName() const;
        
        virtual llvm::Value* codegen(CodeGenContext& context);
    };
}

#endif // IMAGINARY_INTEGER_LITERAL_H
