#include "entity_reference.h"
#include "../../parser/parser.h"
#include <assert.h>

namespace ast
{
    entity_reference::entity_reference(chime::parser* parser)
    {
        assert(parser->look_ahead()->is_identifier());
        this->identifier(parser->next_token_value());
    }
    
    std::string entity_reference::node_name(void)
    {
        return std::string("entity reference");
    }
    std::string entity_reference::to_string(void)
    {
        std::string s;
        
        s += "entity: " + this->identifier();
        
        return s;
    }
    
    std::string entity_reference::identifier() const
    {
        return _identifier;
    }
    void entity_reference::identifier(std::string s)
    {
        assert(!s.empty());
        
        _identifier = s;
    }
}