// chime: AndOperator.h

#ifndef AND_OPERATOR_H
#define AND_OPERATOR_H

#include "binary_operator.h"

namespace chime
{
    class AndOperator : public BinaryOperator
    {
    public:
        std::string nodeName() const;
        
        llvm::Value* codegen(chime::CodeGenContext& context);
    };
}

#endif // AND_OPERATOR_H
