// chime: OrOperator.h

#ifndef OR_OPERATOR_H
#define OR_OPERATOR_H

#include "binary_operator.h"

namespace chime
{
    class OrOperator : public BinaryOperator
    {
    public:
        std::string nodeName() const;
        
        llvm::Value* codegen(chime::CodeGenContext& context);
    };
}

#endif // OR_OPERATOR_H
