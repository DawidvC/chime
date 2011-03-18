#include "type_reference.h"

namespace ast
{
    type_reference::type_reference(chime::parser* parser)
    {
        this->identifier(parser->next_token_value());
    }
    
    std::string type_reference::node_name(void)
    {
        return std::string("type reference");
    }
    std::string type_reference::to_string(void)
    {
        std::string s;
        
        s += "type: " + this->identifier();
        
        return s;
    }
    
    std::string type_reference::identifier() const
    {
        return _identifier;
    }
    void type_reference::identifier(std::string s)
    {
        _identifier = s;
    }
}