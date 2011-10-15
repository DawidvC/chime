#include "AndOperator.h"

namespace chime
{
    std::string AndOperator::nodeName() const
    {
        return "And Operator";
    }
    
    llvm::Value* AndOperator::codegen(CodeGenContext& context)
    {
        return NULL;
    }
}
