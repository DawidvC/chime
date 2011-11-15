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
        
        // load it, so we can return the result
        value = generator.builder()->CreateLoad(value, "return value load");
        
        // after getting a reference to our return value, above, we need
        // to clear out all of our scope references, all the way up to
        // the enclosing function
        
        // But (!), we do not want to release the variable we are returning,
        // so we can save doing a retain on it as well
        std::vector<std::string> identifiersToSkip;
        
        identifiersToSkip.push_back(this->returnVariable()->getIdentifier());
        
        generator.getCurrentScope()->codegenFunctionExit(generator, identifiersToSkip);
        
        generator.builder()->CreateRet(value);
        
        return NULL;
    }
}
