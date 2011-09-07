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
        llvm::Value*      value;
        llvm::Value*      self;
        llvm::Value*      variableNamePtr;
        llvm::Value*      objectPtrPtr;
        llvm::Value*      objectPtr;
        llvm::AllocaInst* alloca;
        
        self = generator.getMethodScope()->getSelfPointer();
        
        variableNamePtr = generator.make_constant_string(this->getIdentifier());
        
        // get the stored object** from chime_closure_get_attribute
        value = generator.getRuntime()->callChimeClosureGetAttribute(self, variableNamePtr);
        
        alloca = generator.builder()->CreateAlloca(generator.getRuntime()->getChimeObjectPtrType(), 0, "closure attribute");
        
        objectPtrPtr = generator.builder()->CreateLoad(value, "closure attribute**");
        objectPtr    = generator.builder()->CreateLoad(objectPtrPtr, "closure attribute*");
        
        generator.builder()->CreateStore(objectPtr, alloca, false);
        
        return alloca;
    }
}
