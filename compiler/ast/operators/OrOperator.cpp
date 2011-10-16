#include "OrOperator.h"

namespace chime
{
    std::string OrOperator::nodeName() const
    {
        return "Or Operator";
    }
    
    llvm::Value* OrOperator::codegen(CodeGenContext& context)
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
        
        ifLeftValueTrueBlock  = llvm::BasicBlock::Create(context.getContext(), "or.left.true", function);
        ifLeftValueFalseBlock = llvm::BasicBlock::Create(context.getContext(), "or.left.false");
        endBlock              = llvm::BasicBlock::Create(context.getContext(), "and.end");
        
        context.builder()->CreateCondBr(leftCondition, ifLeftValueTrueBlock, ifLeftValueFalseBlock);
        
        context.builder()->SetInsertPoint(ifLeftValueTrueBlock);
        
        leftValue = context.builder()->CreateLoad(leftValue, "or.left.value");
        context.builder()->CreateStore(leftValue, result, false);
        
        context.builder()->CreateBr(endBlock);
        
        function->getBasicBlockList().push_back(ifLeftValueFalseBlock);
        context.builder()->SetInsertPoint(ifLeftValueFalseBlock);
        
        rightValue = this->getRightOperand()->codegen(context);
        rightValue = context.builder()->CreateLoad(rightValue, "or.right.value");
        context.builder()->CreateStore(rightValue, result, false);
        
        context.builder()->CreateBr(endBlock);
        
        function->getBasicBlockList().push_back(endBlock);
        context.builder()->SetInsertPoint(endBlock);
        
        return result;
    }
}
