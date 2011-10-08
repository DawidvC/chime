#include "Conditional.h"

namespace ast
{
    NodeRef Conditional::getCondition() const
    {
        return _condition;
    }
    
    void Conditional::setCondition(NodeRef node)
    {
        _condition = node;
    }
    
    NodeRef Conditional::getBody(void) const
    {
        return _body;
    }
    void Conditional::setBody(NodeRef node)
    {
        _body = node;
    }
    
    NodeRef Conditional::getElse(void) const
    {
        return _else;
    }
    void Conditional::setElse(NodeRef node)
    {
        _else = node;
    }
    
    llvm::Value* Conditional::codegenConditional(chime::code_generator& generator, llvm::BasicBlock* endBlock)
    {
        llvm::Value*      conditionValue;
        llvm::Function*   function;
        llvm::BasicBlock* thenBlock;
        llvm::BasicBlock* elseBlock;
        
        // codegen the condition...
        conditionValue = this->getCondition()->codegen(generator);
        
        // ... and wrap it up in a conditional test
        conditionValue = generator.createCondition(conditionValue);
        
        function = generator.builder()->GetInsertBlock()->getParent();
        
        thenBlock = llvm::BasicBlock::Create(generator.get_context(), "conditional.then", function);
        elseBlock = llvm::BasicBlock::Create(generator.get_context(), "conditional.else");
        
        generator.builder()->CreateCondBr(conditionValue, thenBlock, elseBlock);
        
        // setup the then block
        generator.builder()->SetInsertPoint(thenBlock);
        
        this->getBody()->codegen(generator);
        
        // branch to the end, unless we've encountered another terminator
        // in the process of codegen (like a return statement)
        if (!generator.builder()->GetInsertBlock()->getTerminator())
        {
            generator.builder()->CreateBr(endBlock);
        }
        
        function->getBasicBlockList().push_back(elseBlock);
        generator.builder()->SetInsertPoint(elseBlock);
        
        // codegen the else block.  If there is no else, we just have it
        // branch right to the end
        if (this->getElse())
        {
            this->getElse()->codegen(generator);
        }
        
        return NULL;
    }
}
