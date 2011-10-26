#include "SelfLiteral.h"

namespace chime
{
    SelfLiteral::SelfLiteral()
    {
    }
    
    std::string SelfLiteral::nodeName(void) const
    {
        return std::string("self literal");
    }
    
    llvm::Value* SelfLiteral::codegen(chime::code_generator& generator)
    {
        return generator.getCurrentScope()->getSelfValue(generator);
    }
}
