#include "Case.h"
#include "compiler/ast/common/CodeBlock.h"

namespace ast
{
    ast::CaseRef Case::parse(chime::parser& parser, Node* switchExpression)
    {
        ast::Case*           node;
        ast::BinaryOperator* op;
        
        parser.next_token("case");
        
        node = new ast::Case();
        
        op = new ast::BinaryOperator();
        op->setIdentifier("===");
        op->setLeftOperand(switchExpression);
        op->setRightOperand(parser.parse_expression());
        
        node->setCondition(ast::NodeRef(op));
        
        parser.advanceToNextStatement();
        
        node->setBody(ast::NodeRef(ast::CodeBlock::parse(parser)));
        
        parser.advanceToNextStatement();
        
        return ast::CaseRef(node);
    }
    
    std::string Case::nodeName(void) const
    {
        return std::string("Case");
    }
}
