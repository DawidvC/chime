#include "ClosedLocalVariable.h"
#include "compiler/ast/operators/ClosedLocalVariableAssignmentOperator.h"

namespace ast
{
    ClosedLocalVariable::ClosedLocalVariable(const std::string& identifier) :
        Variable(identifier)
    {
    }
    
    std::string ClosedLocalVariable::nodeName(void) const
    {
        return std::string("Closed Local Variable");
    }
    
    AssignmentOperator* ClosedLocalVariable::createAssignment()
    {
        return new ClosedLocalVariableAssignmentOperator();
    }
    
    llvm::Value* ClosedLocalVariable::codegen(chime::code_generator& generator)
    {
        llvm::Value* closure;
        llvm::Value* environment;
        llvm::Value* variableNamePtr;
        
        variableNamePtr = generator.make_constant_string(this->getIdentifier());
        closure         = generator.getMethodScope()->getSelfPointer();
        environment     = generator.getRuntime()->callChimeClosureGetEnvironment(closure);
        
        return generator.getRuntime()->callChimeObjectGetAttribute(environment, variableNamePtr);
    }
}
