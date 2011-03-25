#include "type_reference.h"
#include "../../parser/parser.h"
#include <assert.h>

namespace ast
{
    type_reference::type_reference(chime::parser* parser)
    {
        assert(parser->look_ahead()->is_type());
        
        this->identifier(parser->next_token_value());
        
        if (this->identifier() == "Function")
        {
            // ( parameters )
            parser->next_token_value("(");
            this->parse_parameters(parser);
            parser->next_token_value(")");
        }
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
    
    void type_reference::parse_parameters(chime::parser* parser)
    {
        
    }
}