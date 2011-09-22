#include "InterpolatedString.h"
#include "string_literal.h"
#include "compiler/ast/operators/binary_operator.h"
#include "compiler/ast/operators/MethodCall.h"

namespace ast
{
    ast::Node* InterpolatedString::parse(chime::parser& parser, ast::Node* leftOperand)
    {
        chime::token*         t;
        ast::binary_operator* addOp;
        ast::binary_operator* callOp;
        ast::MethodCall*      toStringCall;
        ast::Node*            node;
        
        t = parser.look_ahead();
        if (t->empty())
            return leftOperand;
        
        if (t->isInterpolatedStringEnd())
        {
            addOp = new ast::binary_operator();
            addOp->identifier("+");
            addOp->setLeftOperand(leftOperand);
            addOp->setRightOperand(ast::string_literal::parse(parser));
            
            return addOp;
        }
        
        if (!t->isInterpolatedStringStart() && !t->isInterpolatedStringMiddle())
        {
            assert(0 && "How did we get here while parsing an interpolated string?");
            
            return NULL;
        }
        
        if (t->isInterpolatedStringStart())
            assert(leftOperand == NULL);
        
        node = ast::string_literal::parse(parser);
        
        // the interpolated expression
        toStringCall = new ast::MethodCall();
        toStringCall->identifier("to_string");
        
        callOp = new ast::binary_operator();
        callOp->identifier(".");
        callOp->setLeftOperand(parser.parse_expression());
        callOp->setRightOperand(toStringCall);
        
        addOp = new ast::binary_operator();
        addOp->identifier("+");
        addOp->setLeftOperand(node);
        addOp->setRightOperand(callOp);
        
        return InterpolatedString::parse(parser, addOp);
    }
    
    std::string InterpolatedString::nodeName(void) const
    {
        return std::string("Interpolated String");
    }
}
