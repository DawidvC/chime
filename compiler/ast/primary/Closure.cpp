#include "Closure.h"
#include "compiler/ast/common/code_block.h"
#include "compiler/codegen/code_generator.h"

namespace ast
{
    Closure::Closure(chime::parser& parser)
    {
        // method identifier
        parser.next_token_value("do");
        
        if (parser.look_ahead()->equal_to("("))
        {
            _parameters = ParameterSetRef(new ParameterSet(parser));
        }
        
        _bodyBlock = CodeBlock::nextBlock(parser);
    }
    
    std::string Closure::nodeName(void) const
    {
        return std::string("Closure");
    }
    
    NodeRef Closure::getBody() const
    {
        return _bodyBlock;
    }
    
    ParameterSetRef Closure::getParameters() const
    {
        return _parameters;
    }
    
    llvm::Value* Closure::codegen(chime::code_generator& generator)
    {
        // This code is very very similar to the code in FunctionDefinition.cpp.  I'd like to
        // figure out a way to share it better
        
        llvm::Function*                function;
        llvm::FunctionType*            functionType;
        std::vector<const llvm::Type*> functionArgs;
        std::string                    functionName;
        llvm::Value*                   closureValue;
        llvm::BasicBlock*              basicBlock;
        llvm::BasicBlock*              currentBlock;
        
        // capture the current insertion point
        currentBlock = generator.builder()->GetInsertBlock();
        
        functionName = generator.getMethodScope()->createNewAnonymousFunctionName();
        
        // returns nothing, takes no arguments
        functionType = llvm::FunctionType::get(generator.getRuntime()->getChimeObjectPtrType(), functionArgs, false);
        
        function = generator.createFunction(functionType, functionName);
        
        basicBlock = llvm::BasicBlock::Create(generator.getContext(), "entry", function, 0);
        generator.builder()->SetInsertPoint(basicBlock);
        
        this->getBody()->codegen(generator);
        
        // only do this if we haven't yet terminated the block
        if (!generator.builder()->GetInsertBlock()->getTerminator())
        {
            generator.builder()->CreateRet(generator.getRuntime()->getChimeLiteralNull());
        }
        
        llvm::verifyFunction(*function);
        
        // restore the builder's position
        generator.builder()->SetInsertPoint(currentBlock);
        
        closureValue = generator.getRuntime()->callChimeClosureCreate(function);
        
        return closureValue;
    }
}