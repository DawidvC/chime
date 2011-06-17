#include "implementation.h"
#include "../../parser/parser.h"
#include <assert.h>

namespace ast
{
    implementation::implementation(chime::parser* parser) : code_container()
    {
        // parse the import statement
        parser->next_token_value("implementation");
        
        this->type_ref(new ast::type_reference(parser));
        
        // check for ": SuperClass"
        if (parser->advance_token_if_equals(":"))
        {
            this->super_class(new ast::type_reference(parser));
        }
        else
        {
            this->super_class(NULL);
        }
        
        // { body }
        this->parse_body(parser, true);
    }
    
    implementation::~implementation()
    {
        delete _type_ref;
        
        if (_super_class)
            delete _super_class;
    }
    
    std::string implementation::node_name(void) const
    {
        return std::string("implementation");
    }
    
    std::string implementation::to_string(void) const
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
    void implementation::type_ref(ast::type_reference* node)
    {
        assert(node != NULL);
        
        _type_ref = node;
    }
    
    ast::type_reference* implementation::super_class() const
    {
        return _super_class;
    }
    void implementation::super_class(ast::type_reference* node)
    {
        _super_class = node;
    }
}
