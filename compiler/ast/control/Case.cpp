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
    
    llvm::Value* Case::codegen(chime::code_generator& generator)
    {
        // llvm::Value*      conditionValue;
        // llvm::Function*   function;
        // llvm::BasicBlock* thenBlock;
        // 
        // // codegen the condition...
        // conditionValue = this->getCondition()->codegen(generator);
        // 
        // // ... and wrap it up in a conditional test
        // conditionValue = generator.createCondition(conditionValue);
        // 
        // function = generator.builder()->GetInsertBlock()->getParent();
        // 
        // thenBlock  = llvm::BasicBlock::Create(generator.get_context(), "if.then", function);
        // 
        // generator.builder()->CreateCondBr(conditionValue, thenBlock, elseBlock);
        // 
        // // setup the then block
        // generator.builder()->SetInsertPoint(thenBlock);
        // 
        // thenValue = this->getBody()->codegen(generator);
        // 
        // // branch to the end, unless we've encountered another terminator
        // // in the process of codegen (like a return statement)
        // if (!generator.builder()->GetInsertBlock()->getTerminator())
        // {
        //     generator.builder()->CreateBr(endBlock);
        // }
        // 
        // function->getBasicBlockList().push_back(elseBlock);
        // generator.builder()->SetInsertPoint(elseBlock);
        // 
        // // codegen the else block.  If there is no else, we just have it
        // // branch right to the end
        // elseValue = NULL;
        // if (this->getElse())
        // {
        //     elseValue = this->getElse()->codegen(generator);
        // }
        // 
        // // guard against duplicate terminators again here
        // if (!generator.builder()->GetInsertBlock()->getTerminator())
        // {
        //     generator.builder()->CreateBr(endBlock);
        // }
        // 
        // elseBlock = generator.builder()->GetInsertBlock();
        // 
        // function->getBasicBlockList().push_back(endBlock);
        // generator.builder()->SetInsertPoint(endBlock);
        
        return NULL;
    }
}
