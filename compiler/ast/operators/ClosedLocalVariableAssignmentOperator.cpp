#include "ClosedLocalVariableAssignmentOperator.h"

namespace ast
{
    std::string ClosedLocalVariableAssignmentOperator::nodeName(void) const
    {
        return std::string("Closed Local Variable Assignment Operator");
    }
    
    llvm::Value* ClosedLocalVariableAssignmentOperator::codegen(chime::code_generator& generator)
    {
        llvm::Value*      rValue;
        llvm::Value*      closure;
        llvm::Value*      attributeNameCStringPtr;
        llvm::AllocaInst* allocaPtrPtr;
        
        // first, codegen the thing we're going to be assigning to the variable
        rValue = this->getRightOperand()->codegen(generator);
        assert(rValue);
        
        attributeNameCStringPtr = generator.make_constant_string(this->getVariable()->getIdentifier());
        closure                 = generator.getMethodScope()->getSelfPointer();
        
        allocaPtrPtr = generator.builder()->CreateAlloca(generator.getRuntime()->getChimeObjectPtrPtrType(), 0, "chime_closure_set_attribute arg1");
        
        generator.builder()->CreateStore(rValue, allocaPtrPtr, false);
        
        generator.getRuntime()->callChimeClosureSetAttribute(closure, attributeNameCStringPtr, allocaPtrPtr);
        
        return rValue;
    }
}
