#include "InstanceVariable.h"
#include "compiler/ast/operators/InstanceVariableAssignmentOperator.h"

namespace ast
{
    InstanceVariable::InstanceVariable(const std::string& identifier) :
        Variable(identifier)
    {
    }
    
    std::string InstanceVariable::nodeName(void) const
    {
        return std::string("Instance Variable");
    }
    
    AssignmentOperator* InstanceVariable::createAssignment()
    {
        return new InstanceVariableAssignmentOperator();
    }
    
    llvm::Value* InstanceVariable::codegen(chime::code_generator& generator)
    {
        llvm::Value* self;
        llvm::Value* attributeNameCStringPtr;
        
        attributeNameCStringPtr = generator.make_constant_string(this->getIdentifier());
        
        self = generator.getMethodScope()->getSelfPointer();
        
        return generator.getRuntime()->callChimeObjectGetAttribute(self, attributeNameCStringPtr);
    }
}