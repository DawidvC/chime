#include "InstanceVariableAssignmentOperator.h"

namespace ast
{
    std::string InstanceVariableAssignmentOperator::nodeName(void) const
    {
        return std::string("Instance Variable Assignment Operator");
    }
    
    llvm::Value* InstanceVariableAssignmentOperator::codegen(chime::code_generator& generator)
    {
        llvm::Value* rValue;
        llvm::Value* self;
        llvm::Value* attributeNameCStringPtr;
        
        // first, codegen the thing we're going to be assigning to the variable
        rValue = this->getRightOperand()->codegen(generator);
        assert(rValue);
        
        attributeNameCStringPtr = generator.getConstantString(this->getVariable()->getIdentifier());
        self                    = generator.getCurrentScope()->selfValue(generator);
        
        generator.getRuntime()->callChimeObjectSetAttribute(self, attributeNameCStringPtr, rValue);
        
        // the return of this statement isn't super obvious
        return rValue;
    }
}
