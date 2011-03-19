#include "method_parameter.h"
#include "../primary/type_reference.h"

namespace ast
{
    method_parameter::method_parameter(chime::parser* parser)
    {
        chime::token* t;
        
        _type = NULL;
        _label = "";
        
        t = parser->look_ahead();
        
        if (t->is_type())
        {
            this->type(new type_reference(parser));
        }
        
        this->identifier(parser->next_token_value());
    }
    
    method_parameter::~method_parameter()
    {
        if (_type)
            delete _type;
    }
    
    std::string method_parameter::node_name(void)
    {
        return std::string("method parameter");
    }
    std::string method_parameter::to_string(void)
    {
        std::string s;
        
        s += "param: " + this->identifier() + "(";
        
        if (!_label.empty())
        {
            s += _label;
        }
        
        if (_type)
        {
            s += " " + _type->identifier();
        }
        
        s += ")";
        
        return s;
    }
    
    std::string method_parameter::identifier() const
    {
        return _identifier;
    }
    void method_parameter::identifier(std::string s)
    {
        _identifier = s;
    }
    
    std::string method_parameter::label() const
    {
        return _label;
    }
    void method_parameter::label(std::string s)
    {
        _label = s;
    }
    ast::type_reference* method_parameter::type() const
    {
        return _type;
    }
    void method_parameter::type(ast::type_reference* t)
    {
        _type = t;
    }
}
