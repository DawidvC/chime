#include "Literal.h"
#include "boolean_literal.h"
#include "integer_literal.h"
#include "string_literal.h"
#include "SelfLiteral.h"
#include "compiler/ast/operators/IndexOperator.h"

namespace ast
{
    ast::node* Literal::parse(chime::parser& parser)
    {
        chime::token* t;
        ast::node*    node;
        
        t = parser.look_ahead();
        if (t->isFloatingPoint())
        {
            assert(0 && "Floating point numbers not parsed yet");
        }
        else if (t->isInteger())
        {
            node = new ast::integer_literal(&parser);
        }
        else if (t->isString())
        {
            node = new ast::string_literal(&parser);
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