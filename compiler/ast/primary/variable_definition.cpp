#include "variable_definition.h"

namespace ast
{
    variable_definition::variable_definition(chime::parser* parser, ast::type_reference* type)
    {
        assert(parser->look_ahead()->is_identifier());
        
        this->identifier(parser->next_token_value());
        this->variable_type(type);
        
        parser->advance_past_ending_tokens();
    }
    
    variable_definition::~variable_definition()
    {
        delete _type_ref;
    }
    
    std::string variable_definition::node_name(void) const
    {
        return std::string("variable definition");
    }
    std::string variable_definition::to_string(void) const
    {
        std::string s;
        
        s += "variable definition: " + this->identifier() + " (" + this->variable_type()->identifier() + ")";
        
        return s;
    }
    
    ast::type_reference* variable_definition::variable_type() const
    {
        return _type_ref;
    }
    void variable_definition::variable_type(ast::type_reference* n)
    {
        assert(n != NULL);
        
        _type_ref = n;
    }
    
    std::string variable_definition::identifier() const
    {
        return _identifier;
    }
    void variable_definition::identifier(std::string s)
    {
        _identifier = s;
    }
}