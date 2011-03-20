#include "method_call.h"
#include "../../parser/parser.h"

namespace ast
{
    method_call::method_call(chime::parser* parser)
    {
        this->identifier(parser->next_token_value());
        
        // ( arguments )
        parser->next_token_value("(");
        this->parse_arguments(parser);
        parser->next_token_value(")");
        
        fprintf(stderr, "parsed a method call\n");
    }
    
    std::string method_call::node_name(void)
    {
        return std::string("method call");
    }
    std::string method_call::to_string(void)
    {
        std::string s;
        
        s += "method call: " + this->identifier();
        
        return s;
    }
    
    std::string method_call::identifier() const
    {
        return _identifier;
    }
    void method_call::identifier(std::string s)
    {
        _identifier = s;
    }
    
    void method_call::parse_arguments(chime::parser* parser)
    {
        
    }
}