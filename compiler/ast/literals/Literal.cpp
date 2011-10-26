#include "Literal.h"
#include "ArrayLiteral.h"
#include "boolean_literal.h"
#include "HashLiteral.h"
#include "ImaginaryIntegerLiteral.h"
#include "integer_literal.h"
#include "InterpolatedString.h"
#include "NilLiteral.h"
#include "RangeLiteral.h"
#include "string_literal.h"
#include "SelfLiteral.h"
#include "compiler/ast/operators/IndexOperator.h"

namespace ast
{
    ast::node* Literal::parse(chime::parser& parser)
    {
        chime::token* t;
        ast::node*    node;
        
        node = NULL;
        t    = parser.lookAhead();
        
        if (t->isFloatingPoint())
        {
            assert(0 && "Floating point numbers not parsed yet");
        }
        else if (t->isInteger())
        {
            if (parser.lookAhead(2)->equal_to(":"))
            {
                node = new chime::RangeLiteral();
                
                static_cast<chime::RangeLiteral*>(node)->setStartValue(parser.nextToken()->integerValue());
                
                parser.nextTokenValue(":");
                
                static_cast<chime::RangeLiteral*>(node)->setEndValue(parser.nextToken()->integerValue());
            }
            else if (t->isImaginary())
            {
                node = new chime::ImaginaryIntegerLiteral();
                
                static_cast<chime::IntegerLiteral*>(node)->setValue(parser.nextToken()->integerValue());
            }
            else
            {
                node = new chime::IntegerLiteral();
                
                static_cast<chime::IntegerLiteral*>(node)->setValue(parser.nextToken()->integerValue());
            }
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
            if (parser.advanceTokenIfEqual("self"))
            {
                node = parser.getCurrentScope()->createSelf();
            }
            else
            {
                assert(0 && "super not parsed yet");
            }
        }
        else if (t->equal_to("nil"))
        {
            parser.nextTokenValue("nil");
            
            node = new chime::NilLiteral();
        }
        else if (t->equal_to("["))
        {
            node = chime::ArrayLiteral::parse(parser);
        }
        else if (t->equal_to("{"))
        {
            node = chime::HashLiteral::parse(parser);
        }
        else
        {
            assert(0 && "Unhandled literal!");
        }
        
        node = ast::IndexOperator::parse(parser, node, true);
        
        return node;
    }
}