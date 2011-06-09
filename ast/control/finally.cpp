// chime: finally.cpp

#include "ast/control/finally.h"
#include "parser/parser.h"

namespace ast
{
    Finally::Finally(chime::parser& parser)
    {
        parser.next_token("finally");
        
        this->parseBody(parser);
    }
    
    std::string Finally::nodeName(void)
    {
        return std::string("finally");
    }
}
