#include "type_reference.h"
#include "parser/parser.h"
#include <assert.h>
#include "operations/code_generator.h"

namespace ast
{
    type_reference::type_reference(chime::parser* parser)
    {
        std::string str;
        
        assert(parser->look_ahead()->is_type());
        
        str += parser->next_token_value();
        
        while (parser->look_ahead()->equal_to(".") && parser->look_ahead(2)->is_type())
        {
            parser->next_token_value(".");
            
            str += ".";
            str +=parser->next_token_value();
        }
        
        this->identifier(str);
        
        if (this->identifier() == "Function")
        {
            // ( parameters )
            parser->next_token_value("(");
            this->parse_parameters(parser);
            parser->next_token_value(")");
        }
    }
    
    std::string type_reference::node_name(void)
    {
        return std::string("type reference");
    }
    std::string type_reference::to_string(void)
    {
        std::string s;
        
        s += "type: " + this->identifier();
        
        return s;
    }
    
    std::string type_reference::identifier() const
    {
        return _identifier;
    }
    void type_reference::identifier(std::string s)
    {
        _identifier = s;
    }
    
    void type_reference::parse_parameters(chime::parser* parser)
    {
    }
    
    llvm::Value* type_reference::codegen(chime::code_generator& generator)
    {
        llvm::Value* class_name_ptr;
        
        class_name_ptr = generator.make_constant_string(this->identifier());
        
        return generator.call_chime_runtime_get_class(class_name_ptr);
    }
}