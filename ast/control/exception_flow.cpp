// chime: exception_flow.cpp

#include "ast/control/exception_flow.h"
#include "parser/parser.h"

namespace ast
{
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
                parser.addError("ExceptionFlow::parseBody: unable to get next node");
                break;
            }
            
            this->add_child(node);
        }
        
        parser.advance_past_ending_tokens();
        parser.next_token_value("}");
        parser.advance_past_ending_tokens();
    }
}