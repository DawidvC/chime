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
    
    ast::NodeRef Case::getCondition() const
    {
        return _condition;
    }
    
    void Case::setCondition(ast::NodeRef node)
    {
        _condition = node;
    }
    
    ast::NodeRef Case::getBody(void) const
    {
        return _body;
    }
    void Case::setBody(ast::NodeRef node)
    {
        _body = node;
    }
    
    llvm::Value* Case::codegenIfBlock(chime::code_generator& generator, llvm::BasicBlock* elseBlock, llvm::BasicBlock* endBlock)
    {
        return NULL;
    }
}
