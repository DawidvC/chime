#include "SelfLiteral.h"

namespace chime
{
    SelfLiteral::SelfLiteral()
    {
    }
    
    std::string SelfLiteral::nodeName() const
    {
        return "self literal";
    }
    
    bool SelfLiteral::isSelf() const
    {
        return true;
    }
    
    llvm::Value* SelfLiteral::codegen(chime::code_generator& generator)
    {
        return generator.getCurrentScope()->selfValue(generator);
    }
}
