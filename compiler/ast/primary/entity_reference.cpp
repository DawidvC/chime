#include "entity_reference.h"

namespace ast
{
    entity_reference::entity_reference()
    {
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
        _identifier = s;
    }
}