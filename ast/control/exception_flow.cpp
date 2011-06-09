// chime: exception_flow.cpp

#include "ast/control/exception_flow.h"
#include "parser/parser.h"

namespace ast
{
    std::string ExceptionFlow::node_name(void)
    {
        return this->nodeName();
    }
    
    void ExceptionFlow::parseBody(chime::parser& parser)
    {
        ast::node* node;
        
        parser.advance_past_ending_tokens();
        parser.next_token_value("{");
        
        while (true)
        {
            parser.advance_past_ending_tokens();
            
            // we've reached the end of the body
            if (parser.look_ahead()->is_ending())
                break;
            
            node = parser.parse_without_structural();
            
            if (!node)
            {
                chime::parse_error* e;
                
                e = new chime::parse_error("try::parseBody: unable to get next node");
                
                parser.add_error(e);
                break;
            }
            
            this->add_child(node);
        }
        
        parser.advance_past_ending_tokens();
        parser.next_token_value("}");
        parser.advance_past_ending_tokens();
    }
}