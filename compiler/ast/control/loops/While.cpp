#include "While.h"
#include "compiler/ast/common/CodeBlock.h"

namespace chime
{
    While* While::parse(chime::parser& parser)
    {
        While* node;
        
        parser.nextToken("while");
        
        node = new While();
        
        node->setCondition(parser.parseExpression());
        
        parser.advanceToNextStatement();
        
        parser.pushScope(node);
        
        node->setBody(ast::CodeBlock::parseBlockWithOptionalBraces(parser));
        
        parser.advanceToNextStatement();
        
        parser.popScope();
        
        return node;
    }
    
    std::string While::nodeName(void) const
    {
        return std::string("While");
    }
    
    llvm::Value* While::codegen(chime::code_generator& generator)
    {
        llvm::Function*   function;
        llvm::BasicBlock* bodyBlock;
        llvm::Value*      conditionValue;
        
        function = generator.builder()->GetInsertBlock()->getParent();
        
        bodyBlock = llvm::BasicBlock::Create(generator.getContext(), "while.body");
        
        this->setStartBlock(llvm::BasicBlock::Create(generator.getContext(), "while.condition", function));
        this->setEndBlock(llvm::BasicBlock::Create(generator.getContext(), "while.end"));
        
        generator.builder()->CreateBr(this->getStartBlock());
        
        generator.builder()->SetInsertPoint(this->getStartBlock());
        
        conditionValue = this->getCondition()->codegen(generator);
        conditionValue = generator.createCondition(conditionValue);
        
        generator.builder()->CreateCondBr(conditionValue, bodyBlock, this->getEndBlock());
        
        function->getBasicBlockList().push_back(bodyBlock);
        generator.builder()->SetInsertPoint(bodyBlock);
        
        generator.pushScope(this);
        
        this->getBody()->codegen(generator);
        
        generator.popScope();
        
        // guard against duplicate terminators again here
        if (!generator.builder()->GetInsertBlock()->getTerminator())
        {
            generator.builder()->CreateBr(this->getStartBlock()); // go back to check the condition
        }
        
        function->getBasicBlockList().push_back(this->getEndBlock());
        generator.builder()->SetInsertPoint(this->getEndBlock());
        
        return NULL;
    }
}