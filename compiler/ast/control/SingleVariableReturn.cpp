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
        llvm::Value* value;
        
        value = this->returnVariable()->codegen(generator);
        
        // I think there is some extra efficiency to be had here, but I've not
        // yet figured out exactly how to extract it.
        
        value = generator.builder()->CreateLoad(value, "return value load");
        
        generator.getCurrentScope()->codegenFunctionExit(generator);
        
        generator.builder()->CreateRet(value);
        
        return NULL;
    }
}
