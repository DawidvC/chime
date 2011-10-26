#include "ClosedSelfLiteral.h"

namespace chime
{
    std::string ClosedSelfLiteral::nodeName() const
    {
        return "Closed Self Literal";
    }

    llvm::Value* ClosedSelfLiteral::codegen(CodeGenContext& context)
    {
        llvm::Value* closureValue;
        
        closureValue = context.getCurrentScope()->getSelfObjectPtr();
        
        return context.getRuntime()->callChimeObjectGetAttribute(closureValue, context.getConstantString("self"));
    }
}
