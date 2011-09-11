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
        
        value = generator.value_for_identifier(this->getIdentifier());
        
        if (!value)
        {
            value = generator.insertChimeObjectAlloca();
            
            generator.set_value_for_identifier(this->getIdentifier(), value);
        }
        
        return value;
    }
}