#include "type_reference.h"
#include "../../parser/parser.h"

namespace ast
{
    type_reference::type_reference(chime::parser* parser)
    {
        std::string   s;
        chime::token* t;
        
        t = parser->look_ahead();
        
        while (t->is_type() || t->equal_to("."))
        {
            t = parser->next_token();
            
            s += t->value;
            
            t = parser->look_ahead();
        }
        
        this->identifier(s);
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