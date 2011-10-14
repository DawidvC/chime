#include "Control.h"
#include "Break.h"
#include "Conditional.h"
#include "Next.h"
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
            node = chime::Next::parse(parser);
            
            node = Conditional::parseTailing(parser, node);
        }
        else if (t->equal_to("break"))
        {
            node = chime::Break::parse(parser);
            
            node = Conditional::parseTailing(parser, node);
        }
        else if (t->equal_to("try"))
        {
            node = new ast::Try(parser);
        }
        else if (t->equal_to("throw"))
        {
            node = new ast::Throw(parser);
            
            node = Conditional::parseTailing(parser, node);
        }
        else if (t->equal_to("if") || t->equal_to("unless"))
        {
            node = chime::Conditional::parse(parser);
        }
        else if (t->equal_to("switch"))
        {
            node = ast::Switch::parse(parser);
        }
        else if (t->equal_to("return"))
        {
            node = new ast::Return(parser);
            
            node = Conditional::parseTailing(parser, node);
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