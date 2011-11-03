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
        
        value = generator.getCurrentScope()->getValueForIdentifier(this->getIdentifier());
        if (!value)
        {
            value = new llvm::GlobalVariable(*generator.module(), 
            /*Type=*/generator.getRuntime()->getChimeObjectPtrType(),
            /*isConstant=*/false,
            /*Linkage=*/llvm::GlobalValue::InternalLinkage,
            /*Initializer=*/0, // initializer set below
            /*Name=*/this->getIdentifier());
            static_cast<llvm::GlobalVariable*>(value)->setAlignment(4);
            static_cast<llvm::GlobalVariable*>(value)->setInitializer(generator.getRuntime()->getChimeLiteralNull());
            
            generator.getCurrentScope()->setValueForIdentifier(this->getIdentifier(), value);
        }
        
        assert(value);
        
        return value;
    }
}
