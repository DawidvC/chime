#include "block.h"
#include "parser/parser.h"

namespace ast
{
    block::block(chime::parser* parser) : code_container()
    {
        // method identifier
        parser->next_token_value("do");
        
        if (parser->look_ahead()->equal_to("("))
        {
            this->parse_parameters(parser);
        }
        
        this->parse_body(parser, false);
    }
    
    std::string block::node_name(void)
    {
        return std::string("block");
    }
}