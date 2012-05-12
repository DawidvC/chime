// chime: finally.cpp

#include "finally.h"

namespace chime
{
    Finally::Finally(chime::parser& parser)
    {
        parser.next_token("finally");
        
        this->parseBody(parser);
    }
    
    std::string Finally::nodeName(void) const
    {
        return std::string("finally");
    }
}
