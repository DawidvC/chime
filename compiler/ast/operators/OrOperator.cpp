#include "OrOperator.h"

namespace chime
{
    std::string OrOperator::nodeName() const
    {
        return "Or Operator";
    }
    
    llvm::Value* OrOperator::codegen(CodeGenContext& context)
    {
        return NULL;
    }
}
