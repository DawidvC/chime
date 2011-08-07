#include "entity_reference.h"

namespace ast
{
    entity_reference::entity_reference()
    {
        _identifier = "";
    }
    
    entity_reference::entity_reference(chime::parser* parser)
    {
        assert(parser->look_ahead()->is_identifier());
        this->identifier(parser->next_token_value());
    }
    
    entity_reference::~entity_reference()
    {
    }
    
    std::string entity_reference::node_name(void) const
    {
        return std::string("entity reference");
    }
    std::string entity_reference::to_string(void) const
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
    
    llvm::Value* entity_reference::codegen(chime::code_generator& generator)
    {
        llvm::Value* value;
        
        value = generator.value_for_identifier(this->identifier());
        
        if (!value)
        {
            value = generator.insert_chime_object_alloca();
            
            generator.set_value_for_identifier(this->identifier(), value);
        }
        
        return value;
    }
}
