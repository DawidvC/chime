#include "SharedLocalVariable.h"
#include "ClosedLocalVariable.h"

namespace chime
{
    SharedLocalVariable::SharedLocalVariable(const std::string& identifier) :
        Variable(identifier)
    {
    }
    
    std::string SharedLocalVariable::nodeName() const
    {
        return "Shared Local Variable";
    }
    
    bool SharedLocalVariable::requiresCapture() const
    {
        return true;
    }
    
    AssignmentOperator* SharedLocalVariable::createAssignment()
    {
        assert(0 && "Assignment to a SharedLocalVariable isn't implemented yet");
        
        return NULL;
    }
    
    Variable* SharedLocalVariable::createClosedVersion()
    {
        return new ClosedLocalVariable(this->identifier());
    }
    
    llvm::Value* SharedLocalVariable::codegenReference(code_generator& generator)
    {
        llvm::Value* reference;
        llvm::Value* variableValue;
        
        // get the value from the current reference
        reference       = generator.getCurrentScope()->getValueForIdentifier(this->getIdentifier());
        variableValue   = generator.getRuntime()->callChimeReferenceGet(reference);
        
        // now, make a new reference with that value
        reference = generator.getRuntime()->callChimeReferenceCreate(variableValue);
        
        return reference;
    }
    
    llvm::Value* SharedLocalVariable::codegen(code_generator& generator)
    {
        llvm::Value* reference;
        
        reference = generator.getCurrentScope()->getValueForIdentifier(this->getIdentifier());
        
        return generator.getRuntime()->callChimeReferenceGet(reference);
    }
}
