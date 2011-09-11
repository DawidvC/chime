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
    
    // AssignmentOperator* ClosedLocalVariable::createAssignment()
    // {
    //     return new ClosedLocalVariableAssignmentOperator();
    // }
    
    llvm::Value* SharedLocalVariable::codegen(chime::code_generator& generator)
    {
        llvm::Value* environment;
        llvm::Value* variableNamePtr;
        
        variableNamePtr = generator.make_constant_string(this->getIdentifier());
        environment     = generator.getMethodScope()->getEnvironmentPointer();
        
        return generator.getRuntime()->callChimeObjectGetAttribute(environment, variableNamePtr);
    }
}
