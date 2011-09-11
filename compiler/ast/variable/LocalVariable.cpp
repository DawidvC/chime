#include "LocalVariable.h"
#include "compiler/ast/operators/LocalVariableAssignmentOperator.h"

namespace ast
{
    LocalVariable::LocalVariable(const std::string& identifier) :
        Variable(identifier)
    {
    }
    
    std::string LocalVariable::nodeName(void) const
    {
        return std::string("Local Variable");
    }
    
    AssignmentOperator* LocalVariable::createAssignment()
    {
        return new LocalVariableAssignmentOperator();
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
