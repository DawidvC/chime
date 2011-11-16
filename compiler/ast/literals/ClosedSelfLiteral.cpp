#include "ClosedSelfLiteral.h"

namespace chime
{
    std::string ClosedSelfLiteral::nodeName() const
    {
        return "Closed Self Literal";
    }
    
    // llvm::Value* ClosedSelfLiteral::codegen(chime::code_generator& generator)
    // {
    //     llvm::Value* closureValue;
    //     
    //     closureValue = generator.getCurrentScope()->selfObjectPtr();
    //     
    //     assert(closureValue);
    //     
    //     return generator.getRuntime()->callChimeObjectGetAttribute(closureValue, generator.getConstantString("self"));
    // }
}
