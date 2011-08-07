#include "next.h"

namespace ast
{
    next::next(chime::parser* parser)
    {
        parser->next_token_value("next");
    }
    
    std::string next::nodeName(void) const
    {
        return std::string("next statement");
    }
}
