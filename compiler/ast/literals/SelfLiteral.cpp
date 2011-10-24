#include "SelfLiteral.h"

namespace ast
{
    SelfLiteral::SelfLiteral(chime::parser& parser)
    {
        parser.next_token_value("self");
    }
    
    std::string SelfLiteral::nodeName(void) const
    {
        return std::string("self literal");
    }
    
    llvm::Value* SelfLiteral::codegen(chime::code_generator& generator)
    {
        return generator.getCurrentScope()->getSelfObjectPtr();
    }
}
