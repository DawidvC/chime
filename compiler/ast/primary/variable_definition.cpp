#include "variable_definition.h"
#include "../../parser/parser.h"
#include <assert.h>

namespace ast
{
    variable_definition::variable_definition(chime::parser* parser)
    {
        chime::token* t;
        
        this->variable_type(new ast::type_reference(parser));
        
        t = parser->next_token();
        
        assert(t->is_identifier());
        
        this->identifier(t->value);
        
        delete t;
    }
    
    variable_definition::~variable_definition()
    {
        delete _type_ref;
    }
    
    std::string variable_definition::node_name(void)
    {
        return std::string("variable definition");
    }
    std::string variable_definition::to_string(void)
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