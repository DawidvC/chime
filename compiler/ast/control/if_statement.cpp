#include "if_statement.h"

namespace chime
{
    std::string IfStatement::nodeName() const
    {
        return std::string("if statement");
    }
    
    llvm::Value* IfStatement::codegen(code_generator& generator)
    {
        llvm::Function*   function;
        llvm::BasicBlock* endBlock;
        llvm::Value*      value;
        
        function = generator.builder()->GetInsertBlock()->getParent();
        
        endBlock = llvm::BasicBlock::Create(generator.getContext(), "if.end");
        
        value = this->codegenConditional(generator, endBlock);
        
        // guard against duplicate terminators again here
        if (!generator.builder()->GetInsertBlock()->getTerminator())
        {
            generator.builder()->CreateBr(endBlock);
        }
        
        function->getBasicBlockList().push_back(endBlock);
        generator.builder()->SetInsertPoint(endBlock);
        
        return value;
    }
}
