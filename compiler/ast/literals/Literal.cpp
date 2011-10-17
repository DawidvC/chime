#include "Literal.h"
#include "boolean_literal.h"
#include "ImaginaryIntegerLiteral.h"
#include "integer_literal.h"
#include "string_literal.h"
#include "SelfLiteral.h"
#include "InterpolatedString.h"
#include "compiler/ast/operators/IndexOperator.h"

namespace ast
{
    ast::node* Literal::parse(chime::parser& parser)
    {
        chime::token* t;
        ast::node*    node;
        
        node = NULL;
        t    = parser.look_ahead();
        
        if (t->isFloatingPoint())
        {
            assert(0 && "Floating point numbers not parsed yet");
        }
        else if (t->isInteger())
        {
            if (t->isImaginary())
                node = new chime::ImaginaryIntegerLiteral();
            else
                node = new chime::IntegerLiteral();
            
            static_cast<chime::IntegerLiteral*>(node)->setValue(parser.nextToken()->integerValue());
        }
        else if (t->isPlainString())
        {
            node = ast::string_literal::parse(parser);
        }
        else if (t->isInterpolatedStringStart())
        {
            node = ast::InterpolatedString::parse(parser);
        }
        else if (t->isBoolean())
        {
            node = new ast::boolean_literal(&parser);
        }
        else if (t->isInheritanceRelated())
        {
            if (t->equal_to("self"))
            {
                node = new ast::SelfLiteral(parser);
            }
            else
            {
                assert(0 && "super not parsed yet");
            }
        }
        else
        {
            assert(0 && "Unhandled literal!");
        }
        
        node = ast::IndexOperator::parse(parser, node);
        
        return node;
    }
}