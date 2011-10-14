#include "Unless.h"

namespace chime
{
    std::string Unless::nodeName() const
    {
        return std::string("Unless");
    }
    
    llvm::Value* Unless::codegen(code_generator& generator)
    {
        llvm::Function*   function;
        llvm::BasicBlock* endBlock;
        llvm::Value*      value;
        
        function = generator.builder()->GetInsertBlock()->getParent();
        
        endBlock = llvm::BasicBlock::Create(generator.getContext(), "unless.end");
        
        value = this->codegenConditional(generator, endBlock, true);
        
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
