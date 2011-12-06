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
    
    llvm::Value* ClosedLocalVariable::codegenReference(chime::code_generator& generator)
    {
        llvm::Value* closure;
        llvm::Value* reference;
        llvm::Value* variableNamePtr;
        
        // this is tricky - we don't want to reference "self", we want to reference the closure itself
        variableNamePtr = generator.getConstantString(this->getIdentifier());
        closure         = generator.getCurrentScope()->selfObjectPtr();
        
        assert(closure);
        assert(variableNamePtr);
        
        reference = generator.getRuntime()->callChimeObjectGetAttribute(closure, variableNamePtr);
        
        generator.getRuntime()->callChimeObjectRetain(reference);
        
        return reference;
    }
    
    llvm::Value* ClosedLocalVariable::codegen(chime::code_generator& generator)
    {
        llvm::Value* reference;
        
        reference = this->codegenReference(generator);
        
        return generator.getRuntime()->callChimeReferenceGet(reference);
    }
}
