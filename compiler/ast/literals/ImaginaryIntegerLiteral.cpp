#include "ImaginaryIntegerLiteral.h"

#include <sstream>

namespace chime
{
    std::string ImaginaryIntegerLiteral::nodeName() const
    {
        return "Imaginary Integer Literal";
    }
    
    llvm::Value* ImaginaryIntegerLiteral::codegen(CodeGenContext& context)
    {
        return NULL;
    }
}
