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
        llvm::Value*  closureValue;
        llvm::Value*  referenceValue;
        llvm::Value*  variableNamePtr;
        
        // first, codegen the thing we're going to be assigning to the variable
        rValue = this->getRightOperand()->codegen(generator);
        assert(rValue);
        
        // tricky - don't want self, want the closure
        variableNamePtr = generator.getConstantString(this->getVariable()->getIdentifier());
        closureValue    = generator.getCurrentScope()->selfObjectPtr();
        referenceValue  = generator.getRuntime()->callChimeObjectGetAttribute(closureValue, variableNamePtr);
        
        generator.getRuntime()->callChimeReferenceSet(referenceValue, rValue);
        
        return rValue;
    }
}
