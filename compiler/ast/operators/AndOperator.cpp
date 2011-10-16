#include "AndOperator.h"

namespace chime
{
    std::string AndOperator::nodeName() const
    {
        return "And Operator";
    }
    
    llvm::Value* AndOperator::codegen(CodeGenContext& context)
    {
        llvm::Value*      leftValue;
        llvm::Value*      rightValue;
        llvm::Value*      leftCondition;
        llvm::Value*      result;
        llvm::Function*   function;
        llvm::BasicBlock* ifLeftValueTrueBlock;
        llvm::BasicBlock* ifLeftValueFalseBlock;
        llvm::BasicBlock* endBlock;
        
        result        = context.insertChimeObjectAlloca();
        leftValue     = this->getLeftOperand()->codegen(context);
        leftCondition = context.createCondition(leftValue);
        assert(leftValue && leftCondition);
        
        function = context.builder()->GetInsertBlock()->getParent();
        
        ifLeftValueTrueBlock  = llvm::BasicBlock::Create(context.getContext(), "and.left.true", function);
        ifLeftValueFalseBlock = llvm::BasicBlock::Create(context.getContext(), "and.left.false");
        endBlock              = llvm::BasicBlock::Create(context.getContext(), "and.end");
        
        context.builder()->CreateCondBr(leftCondition, ifLeftValueTrueBlock, ifLeftValueFalseBlock);
        
        context.builder()->SetInsertPoint(ifLeftValueTrueBlock);
        
        rightValue = this->getRightOperand()->codegen(context);
        assert(rightValue);
        
        rightValue = context.builder()->CreateLoad(rightValue, "and.right.value");
        context.builder()->CreateStore(rightValue, result, false);
        
        context.builder()->CreateBr(endBlock);
        
        function->getBasicBlockList().push_back(ifLeftValueFalseBlock);
        context.builder()->SetInsertPoint(ifLeftValueFalseBlock);
        
        leftValue = context.builder()->CreateLoad(leftValue, "and.left.value");
        context.builder()->CreateStore(leftValue, result, false);
        
        context.builder()->CreateBr(endBlock);
        
        function->getBasicBlockList().push_back(endBlock);
        context.builder()->SetInsertPoint(endBlock);
        
        return result;
    }
}
