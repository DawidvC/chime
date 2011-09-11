#include "ClosedLocalVariableAssignmentOperator.h"

namespace ast
{
    std::string ClosedLocalVariableAssignmentOperator::nodeName(void) const
    {
        return std::string("Closed Local Variable Assignment Operator");
    }
    
    llvm::Value* ClosedLocalVariableAssignmentOperator::codegen(chime::code_generator& generator)
    {
        llvm::Value*  rValue;
        llvm::Value*  closure;
        llvm::Value*  environment;
        llvm::Value*  variableNamePtr;
        
        // first, codegen the thing we're going to be assigning to the variable
        rValue = this->getRightOperand()->codegen(generator);
        assert(rValue);
        
        variableNamePtr = generator.make_constant_string(this->getVariable()->getIdentifier());
        closure         = generator.getMethodScope()->getSelfPointer();
        environment     = generator.getRuntime()->callChimeClosureGetEnvironment(closure);
        
        generator.getRuntime()->callChimeObjectSetAttribute(environment, variableNamePtr, rValue);
        
        return rValue;
    }
}
