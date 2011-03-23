#include "method_definition.h"
#include "../../parser/parser.h"

namespace ast
{
    method_definition::method_definition(chime::parser* parser) : code_container()
    {
        // method identifier
        parser->next_token_value("method");
        this->identifier(parser->next_token_value());
        
        this->parse_parameters(parser);
        this->parse_body(parser);
    }
    
    std::string method_definition::node_name(void)
    {
        return std::string("method definition");
    }
    std::string method_definition::to_string(void)
    {
        std::string s;
        
        s += "method: " + this->identifier();
        
        return s;
    }
    
    std::string method_definition::identifier() const
    {
        return _identifier;
    }
    void method_definition::identifier(std::string s)
    {
        _identifier = s;
    }
}