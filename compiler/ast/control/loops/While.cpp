#include "While.h"
#include "compiler/ast/common/CodeBlock.h"

namespace chime
{
    While* While::parse(chime::parser& parser)
    {
        While* whileNode;
        
        parser.nextToken("while");
        
        whileNode = new While();
        
        whileNode->setCondition(parser.parseExpression());
        
        parser.advanceToNextStatement();
        
        whileNode->setBody(ast::CodeBlock::parseBlockWithOptionalBraces(parser));
        
        parser.advanceToNextStatement();
        
        return whileNode;
    }
    
    std::string While::nodeName(void) const
    {
        return std::string("While");
    }
    
    llvm::Value* While::codegen(chime::code_generator& generator)
    {
        llvm::Function*   function;
        llvm::BasicBlock* conditionBlock;
        llvm::BasicBlock* bodyBlock;
        llvm::BasicBlock* endBlock;
        llvm::Value*      conditionValue;
        
        function = generator.builder()->GetInsertBlock()->getParent();
        
        conditionBlock = llvm::BasicBlock::Create(generator.getContext(), "while.condition", function);
        bodyBlock      = llvm::BasicBlock::Create(generator.getContext(), "while.body");
        endBlock       = llvm::BasicBlock::Create(generator.getContext(), "while.end");
        
        generator.builder()->CreateBr(conditionBlock);
        
        generator.builder()->SetInsertPoint(conditionBlock);
        
        conditionValue = this->getCondition()->codegen(generator);
        conditionValue = generator.createCondition(conditionValue);
        
        generator.builder()->CreateCondBr(conditionValue, bodyBlock, endBlock);
        
        function->getBasicBlockList().push_back(bodyBlock);
        generator.builder()->SetInsertPoint(bodyBlock);
        
        this->getBody()->codegen(generator);
        
        // guard against duplicate terminators again here
        if (!generator.builder()->GetInsertBlock()->getTerminator())
        {
            generator.builder()->CreateBr(conditionBlock); // go back to check the condition
        }
        
        function->getBasicBlockList().push_back(endBlock);
        generator.builder()->SetInsertPoint(endBlock);
        
        return NULL;
    }
}