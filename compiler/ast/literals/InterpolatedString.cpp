#include "InterpolatedString.h"
#include "string_literal.h"
#include "compiler/ast/operators/binary_operator.h"
#include "compiler/ast/operators/MethodCall.h"

namespace chime
{
    // " " + () + " " + () + " "
    Node* InterpolatedString::parse(chime::parser& parser)
    {
        chime::token*   t;
        BinaryOperator* leftOp;
        BinaryOperator* rightOp;
        Node*           node;
        
        t = parser.look_ahead();
        if (t->empty())
            return NULL;
        
        if (t->isInterpolatedStringEnd())
        {
            return string_literal::parse(parser);
        }
        
        node = string_literal::parse(parser);
        
        leftOp = new BinaryOperator();
        leftOp->identifier("+");
        leftOp->setLeftOperand(node);
        leftOp->setRightOperand(parser.parse_expression());
        
        rightOp = new BinaryOperator();
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
