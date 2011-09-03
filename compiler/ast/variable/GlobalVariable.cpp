#include "GlobalVariable.h"

namespace ast
{
    GlobalVariable::GlobalVariable(const std::string& identifier) :
        Variable(identifier)
    {
    }
    
    std::string GlobalVariable::nodeName(void) const
    {
        if (this->getClosed())
            return std::string("Closed Global Variable");
        else
            return std::string("Global Variable");
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