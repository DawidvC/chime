#include "return.h"
#include "parser/parser.h"
#include "codegen/code_generator.h"

namespace ast
{
    Return::Return(chime::parser& parser)
    {
        parser.next_token("return");
        
        // we need to deal with an argument to return here
    }
    
    std::string Return::nodeName(void) const
    {
        return std::string("return");
    }
    
    llvm::Value* Return::codegen(chime::code_generator& generator)
    {
        generator.builder()->CreateBr(generator.getMethodScope()->getReturnBlock());
        
        return NULL;
    }
}
