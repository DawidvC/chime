#include "implementation.h"
#include "method_definition.h"
#include "../../parser/parser.h"

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
        
        // { body }
        parser->next_token_value("{");
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
        ast::node* node;
        
        while (true)
        {
            // this means we've reached the end of the body
            if (parser->look_ahead()->equal_to("}"))
                break;
                
            node = parser->parse_with_structural();
            if (!node)
            {
                chime::parse_error* e;
                
                e = new chime::parse_error("method_definition::parse_body: unable to get next node");
                
                parser->errors()->push_back(e);
                break;
            }
            
            this->add_child(node);
        }
    }
}
