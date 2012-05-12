// chime: throw.cpp

#include "throw.h"

namespace chime
{
    Throw::Throw(chime::parser& parser)
    {
        parser.next_token("throw");
        
        this->add_child(parser.parse_expression());
    }
    
    std::string Throw::nodeName(void) const
    {
        return std::string("throw");
    }
}