// chime: exception_flow.cpp

#include "exception_flow.h"

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
            if (parser.look_ahead()->isBlockEnding())
                break;
            
            node = parser.parse_without_structural();
            
            if (!node)
            {
                parser.addError("ExceptionFlow::parseBody: unable to get next node");
                break;
            }
            
            this->addChild(node);
        }
        
        parser.advance_past_ending_tokens();
        parser.next_token_value("}");
        parser.advance_past_ending_tokens();
    }
}