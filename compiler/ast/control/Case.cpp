#include "Case.h"
#include "compiler/ast/CodeBlock.h"

namespace chime
{
    CaseRef Case::parse(parser& parser, Node* switchExpression)
    {
        Case*                node;
        ast::BinaryOperator* op;
        
        parser.next_token("case");
        
        node = new Case();
        
        op = new ast::BinaryOperator();
        op->setIdentifier("===");
        op->setLeftOperand(switchExpression);
        op->setRightOperand(parser.parse_expression());
        
        node->setCondition(NodeRef(op));
        
        parser.advanceToNextStatement();
        
        node->setBody(NodeRef(CodeBlock::parse(parser)));
        
        parser.advanceToNextStatement();
        
        return CaseRef(node);
    }
    
    std::string Case::nodeName(void) const
    {
        return std::string("Case");
    }
}
