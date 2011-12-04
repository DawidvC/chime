#include "SingleVariableReturn.h"

namespace chime
{
    std::string SingleVariableReturn::nodeName() const
    {
        return "Single Variable Return";
    }
    
    VariableRef SingleVariableReturn::returnVariable() const
    {
        return dynamic_pointer_cast<Variable>(this->getReturnValue());
    }
    
    llvm::Value* SingleVariableReturn::codegen(CodeGenContext& generator)
    {
        llvm::Value*             value;
        std::vector<std::string> identifiersToSkip;
        
        value = this->returnVariable()->codegen(generator);
        
        value = generator.builder()->CreateLoad(value, "return value load");
        
        identifiersToSkip.push_back(this->returnVariable()->getIdentifier());
        
        generator.getCurrentScope()->codegenFunctionExit(generator, identifiersToSkip);
        
        generator.builder()->CreateRet(value);
        
        return NULL;
    }
}
