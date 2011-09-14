#include "SharedLocalVariable.h"

namespace ast
{
    SharedLocalVariable::SharedLocalVariable(const std::string& identifier) :
        Variable(identifier)
    {
    }
    
    std::string SharedLocalVariable::nodeName(void) const
    {
        return std::string("Shared Local Variable");
    }
    
    AssignmentOperator* SharedLocalVariable::createAssignment()
    {
        assert(0 && "Assignment to a SharedLocalVariable isn't implemented yet");
        
        return NULL;
    }
    
    llvm::Value* SharedLocalVariable::codegen(chime::code_generator& generator)
    {
        llvm::Value* reference;
        
        reference = generator.getCurrentScope()->getValueForIdentifier(this->getIdentifier());
        
        return generator.getRuntime()->callChimeReferenceGet(reference);
    }
}
