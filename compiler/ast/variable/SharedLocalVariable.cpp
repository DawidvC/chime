#include "SharedLocalVariable.h"

namespace ast
{
    SharedLocalVariable::SharedLocalVariable(const std::string& identifier) :
        Variable(identifier)
    {
    }
    
    std::string SharedLocalVariable::nodeName(void) const
    {
        return std::string("Shared Local Variable");
    }
    
    AssignmentOperator* SharedLocalVariable::createAssignment()
    {
        assert(0 && "Assignment to a SharedLocalVariable isn't implemented yet");
        
        return NULL;
    }
    
    llvm::Value* SharedLocalVariable::codegen(chime::code_generator& generator)
    {
        llvm::Value* environment;
        llvm::Value* variableNamePtr;
        
        variableNamePtr = generator.getConstantString(this->getIdentifier());
        environment     = generator.getCurrentScope()->getValueForIdentifier("_environment");
        
        return generator.getRuntime()->callChimeObjectGetAttribute(environment, variableNamePtr);
    }
}
