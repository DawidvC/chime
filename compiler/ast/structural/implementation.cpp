#include "implementation.h"

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
        
        // here, we parse the body, somehow
        
        parser->next_token_value("}");
    }
    
    std::string implementation::node_name(void)
    {
        return std::string("implementation");
    }
    
    ast::type_reference* implementation::type_ref() const
    {
        return (ast::type_reference*)this->child_at_index(0);
    }
    void implementation::type_ref(ast::type_reference* n)
    {
        this->add_child(n);
    }
    
    ast::type_reference* implementation::super_class() const
    {
        return (ast::type_reference*)this->child_at_index(1);
    }
    void implementation::super_class(ast::type_reference* n)
    {
        this->add_child(n);
    }
}
