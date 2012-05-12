#include "LocalVariable.h"
#include "ClosedLocalVariable.h"
#include "compiler/ast/operators/LocalVariableAssignmentOperator.h"


namespace chime
{
    LocalVariable::LocalVariable(const std::string& identifier) :
        Variable(identifier)
    {
    }
    
    std::string LocalVariable::nodeName() const
    {
        return "Local Variable";
    }
    
    bool LocalVariable::requiresCapture() const
    {
        return true;
    }
    
    AssignmentOperator* LocalVariable::createAssignment()
    {
        return new LocalVariableAssignmentOperator();
    }
    
    chime::Variable* LocalVariable::createClosedVersion()
    {
        return new ClosedLocalVariable(this->getIdentifier());
    }
    
    llvm::Value* LocalVariable::codegen(chime::code_generator& generator)
    {
        llvm::Value* value;
        
        value = generator.getCurrentScope()->getValueForIdentifier(this->getIdentifier());
        if (!value)
        {
            value = generator.insertChimeObjectAlloca();
            
            generator.getCurrentScope()->setValueForIdentifier(this->getIdentifier(), value);
        }
        
        assert(value);
        
        return value;
    }
}
