#include "implementation.h"
#include "instance_variable.h"

namespace ast
{
    implementation::implementation(chime::parser* parser)
    {
        // parse the import statement
        parser->next_token_value("implementation");
        
        this->type_ref(new ast::type_reference(parser));
        
        // check for ": SuperClass"
        if (parser->advance_token_if_equals(":"))
        {
            this->super_class(new ast::type_reference(parser));
        }
        
        parser->next_token_value("{"); // beginning the body
        
        this->parse_body(parser);
        
        parser->next_token_value("}");
    }
    
    implementation::~implementation()
    {
        delete _type_ref;
        
        if (_super_class)
            delete _super_class;
    }
    
    std::string implementation::node_name(void)
    {
        return std::string("implementation");
    }
    
    std::string implementation::to_string(void)
    {
        std::string s;
        
        s += "implementation: " + this->type_ref()->identifier();
        
        if (this->super_class())
            s += " : " + this->super_class()->identifier();
        
        return s;
    }
    
    ast::type_reference* implementation::type_ref() const
    {
        return _type_ref;
    }
    void implementation::type_ref(ast::type_reference* n)
    {
        _type_ref = n;
    }
    
    ast::type_reference* implementation::super_class() const
    {
        return _super_class;
    }
    void implementation::super_class(ast::type_reference* n)
    {
        _super_class = n;
    }
    
    void implementation::parse_body(chime::parser* parser)
    {
        chime::token* t;
        
        while (true)
        {
            t = parser->look_ahead();
            
            if (t->empty() || t->equal_to("}"))
                break;
            
            if (t->is_type())
            {
                // this means we have an instance variable definition
                this->add_child(new ast::instance_variable(parser));
            }
            else if (t->equal_to("method"))
            {
                fprintf(stderr, "found a method def\n");
                break;
            }
            else
            {
                fprintf(stderr, "found something we weren't expecting in parse_body '%s'\n", t->value.c_str());
                break;
            }
        }
    }
}
