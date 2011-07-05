#include "return.h"
#include "parser/parser.h"

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
}
