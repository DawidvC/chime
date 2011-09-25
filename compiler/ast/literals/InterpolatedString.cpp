#include "InterpolatedString.h"
#include "string_literal.h"
#include "compiler/ast/operators/binary_operator.h"
#include "compiler/ast/operators/MethodCall.h"

namespace ast
{
    // " " + () + " " + () + " "
    ast::Node* InterpolatedString::parse(chime::parser& parser)
    {
        chime::token*        t;
        ast::BinaryOperator* leftOp;
        ast::BinaryOperator* rightOp;
        ast::Node*           node;
        
        t = parser.look_ahead();
        if (t->empty())
            return NULL;
        
        if (t->isInterpolatedStringEnd())
        {
            return ast::string_literal::parse(parser);
        }
        
        node = ast::string_literal::parse(parser);
        
        leftOp = new ast::BinaryOperator();
        leftOp->identifier("+");
        leftOp->setLeftOperand(node);
        leftOp->setRightOperand(parser.parse_expression());
        
        rightOp = new ast::BinaryOperator();
        rightOp->identifier("+");
        rightOp->setLeftOperand(leftOp);
        rightOp->setRightOperand(InterpolatedString::parse(parser));
        
        return rightOp;
    }
    
    std::string InterpolatedString::nodeName(void) const
    {
        return std::string("Interpolated String");
    }
}
