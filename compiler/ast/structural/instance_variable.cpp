#include "instance_variable.h"

namespace ast
{
    instance_variable::instance_variable(chime::parser* parser)
    {
        this->variable_type(new ast::type_reference(parser));
        
        this->identifier(parser->next_token_value());
    }
    
    instance_variable::~instance_variable()
    {
        delete _type_ref;
    }
    
    std::string instance_variable::node_name(void)
    {
        return std::string("instance variable");
    }
    std::string instance_variable::to_string(void)
    {
        std::string s;
        
        s += "instance variable: " + this->identifier() + " (" + this->variable_type()->identifier() + ")";
        
        return s;
    }
    
    ast::type_reference* instance_variable::variable_type() const
    {
        return _type_ref;
    }
    void instance_variable::variable_type(ast::type_reference* n)
    {
        _type_ref = n;
    }
    
    std::string instance_variable::identifier() const
    {
        return _identifier;
    }
    void instance_variable::identifier(std::string s)
    {
        _identifier = s;
    }
}