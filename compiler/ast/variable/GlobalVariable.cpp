#include "GlobalVariable.h"
#include "compiler/ast/operators/GlobalVariableAssignmentOperator.h"

namespace ast
{
    GlobalVariable::GlobalVariable(const std::string& identifier) :
        Variable(identifier)
    {
    }
    
    std::string GlobalVariable::nodeName(void) const
    {
        return std::string("Global Variable");
    }
    
    AssignmentOperator* GlobalVariable::createAssignment()
    {
        return new GlobalVariableAssignmentOperator();
    }
    
    llvm::Value* GlobalVariable::codegen(chime::code_generator& generator)
    {
        llvm::Value* value;
        
        value = generator.value_for_identifier(this->getIdentifier());
        
        if (!value)
        {
            value = generator.insert_chime_object_alloca();
            
            generator.set_value_for_identifier(this->getIdentifier(), value);
        }
        
        return value;
    }
}