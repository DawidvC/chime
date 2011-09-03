#include "entity_reference.h"

namespace ast
{
    ast::node* entity_reference::parse(chime::parser& parser)
    {
        ast::node* node;
        
        node = new entity_reference(&parser);
        
        // We now have an entity created.  But, we need to know
        // the scope this variable is in.
        
        // check the next token for a right-hand-side
        // operator (which is currently only "[")
        // if (parser.look_ahead()->equal_to("["))
        // {
        //     node = ast::IndexOperator::parse(this, node);
        // }
        
        return node;
    }
    
    entity_reference::entity_reference(chime::parser* parser)
    {
        assert(parser->look_ahead()->isIdentifier());
        this->identifier(parser->next_token_value());
    }
    
    entity_reference::~entity_reference()
    {
    }
    
    std::string entity_reference::nodeName(void) const
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
        
        if (generator.isEntityAnInstanceVariable(this->identifier()))
        {
            llvm::Value* self;
            llvm::Value* attributeNameCStringPtr;
            
            attributeNameCStringPtr = generator.make_constant_string(this->identifier());
            
            self = generator.getMethodScope()->getSelfPointer();
            
            return generator.getRuntime()->callChimeObjectGetAttribute(self, attributeNameCStringPtr);
        }
        
        value = generator.value_for_identifier(this->identifier());
        
        if (!value)
        {
            value = generator.insert_chime_object_alloca();
            
            generator.set_value_for_identifier(this->identifier(), value);
        }
        
        return value;
    }
}
