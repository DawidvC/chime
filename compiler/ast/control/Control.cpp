#include "Control.h"
#include "if_statement.h"
#include "next.h"
#include "return.h"
#include "Switch.h"
#include "exceptions/throw.h"
#include "exceptions/try.h"
#include "loops/Until.h"
#include "loops/While.h"

namespace chime
{
    ast::Node* Control::parse(chime::parser& parser)
    {
        Token*     t;
        ast::node* node;
        
        node = NULL;
        
        t = parser.look_ahead();
        if (t->equal_to("next"))
        {
            node = new ast::next(&parser);
            
            node = parser.parse_tailing_conditional(node);
        }
        else if (t->equal_to("try"))
        {
            node = new ast::Try(parser);
        }
        else if (t->equal_to("throw"))
        {
            node = new ast::Throw(parser);
            
            node = parser.parse_tailing_conditional(node);
        }
        else if (t->equal_to("if"))
        {
            node = new ast::IfStatement(parser, NULL);
        }
        else if (t->equal_to("switch"))
        {
            node = ast::Switch::parse(parser);
        }
        else if (t->equal_to("return"))
        {
            node = new ast::Return(parser);
        }
        else if (t->equal_to("while"))
        {
            node = While::parse(parser);
        }
        else if (t->equal_to("until"))
        {
            node = Until::parse(parser);
        }
        else if (t->equal_to("loop"))
        {
            node = Loop::parse(parser);
        }
        else
        {
            assert(false && "Unhandled control statement");
        }
        
        return node;
    }
}