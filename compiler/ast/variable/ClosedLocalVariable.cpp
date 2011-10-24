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
        
        variableNamePtr = generator.getConstantString(this->getIdentifier());
        closure         = generator.getCurrentScope()->getSelfObjectPtr();
        reference       = generator.getRuntime()->callChimeObjectGetAttribute(closure, variableNamePtr);
        
        return reference;
    }
    
    llvm::Value* ClosedLocalVariable::codegen(chime::code_generator& generator)
    {
        llvm::Value* reference;
        
        reference = this->codegenReference(generator);
        
        return generator.getRuntime()->callChimeReferenceGet(reference);
    }
}
