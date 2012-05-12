#include "GlobalVariable.h"
#include "ClosedGlobalVariable.h"
#include "compiler/ast/operators/GlobalVariableAssignmentOperator.h"

namespace chime
{
    GlobalVariable::GlobalVariable(const std::string& identifier) :
        Variable(identifier)
    {
    }
    
    std::string GlobalVariable::nodeName() const
    {
        return "Global Variable";
    }
    
    bool GlobalVariable::requiresCapture() const
    {
        return true;
    }
    
    AssignmentOperator* GlobalVariable::createAssignment()
    {
        return new GlobalVariableAssignmentOperator();
    }
    
    chime::Variable* GlobalVariable::createClosedVersion()
    {
        return new chime::ClosedGlobalVariable(this->identifier());
    }
    
    llvm::Value* GlobalVariable::codegen(chime::code_generator& generator)
    {
        llvm::Value* value;
        
        value = generator.getCurrentScope()->getValueForIdentifier(this->identifier());
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
            
            generator.getCurrentScope()->setValueForIdentifier(this->identifier(), value);
        }
        
        assert(value);
        
        return value;
    }
}
