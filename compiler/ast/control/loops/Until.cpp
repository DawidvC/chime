#include "Until.h"
#include "compiler/ast/common/CodeBlock.h"

namespace chime
{
    Until* Until::parse(chime::parser& parser)
    {
        Until* node;
        
        parser.nextToken("until");
        
        node = new Until();
        
        node->setCondition(parser.parseExpression());
        
        parser.advanceToNextStatement();
        
        parser.pushScope(node);
        
        node->setBody(ast::CodeBlock::parseBlockWithOptionalBraces(parser));
        
        parser.advanceToNextStatement();
        
        parser.popScope();
        
        return node;
    }
    
    std::string Until::nodeName(void) const
    {
        return std::string("Until");
    }
    
    // This is *nearly* identical to the while loop, with the exception of the block names and
    // the flipped order of the blocks for the CreateCondBr call.  Definitely to be refactored.
    llvm::Value* Until::codegen(chime::code_generator& generator)
    {
        llvm::Function*   function;
        llvm::BasicBlock* conditionBlock;
        llvm::BasicBlock* bodyBlock;
        llvm::BasicBlock* endBlock;
        llvm::Value*      conditionValue;
        
        function = generator.builder()->GetInsertBlock()->getParent();
        
        conditionBlock = llvm::BasicBlock::Create(generator.getContext(), "until.condition", function);
        bodyBlock      = llvm::BasicBlock::Create(generator.getContext(), "until.body");
        endBlock       = llvm::BasicBlock::Create(generator.getContext(), "until.end");
        
        generator.builder()->CreateBr(conditionBlock);
        
        generator.builder()->SetInsertPoint(conditionBlock);
        
        conditionValue = this->getCondition()->codegen(generator);
        conditionValue = generator.createCondition(conditionValue);
        
        generator.builder()->CreateCondBr(conditionValue, endBlock, bodyBlock);
        
        function->getBasicBlockList().push_back(bodyBlock);
        generator.builder()->SetInsertPoint(bodyBlock);
        
        generator.pushScope(this);
        
        this->getBody()->codegen(generator);
        
        generator.popScope();
        
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
