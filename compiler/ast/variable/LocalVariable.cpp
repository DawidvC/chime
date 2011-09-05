#include "LocalVariable.h"

namespace ast
{
    LocalVariable::LocalVariable(const std::string& identifier) :
        Variable(identifier)
    {
    }
    
    std::string LocalVariable::nodeName(void) const
    {
        if (this->getClosed())
            return std::string("Closed Local Variable");
        else
            return std::string("Local Variable");
    }
    
    llvm::Value* LocalVariable::codegen(chime::code_generator& generator)
    {
        llvm::Value* value;
        llvm::Value* self;
        
        if (this->getClosed())
        {
            llvm::Value*      variableNamePtr;
            llvm::Value*      objectPtrPtr;
            llvm::Value*      objectPtr;
            llvm::AllocaInst* alloca;
            
            self = generator.getMethodScope()->getSelfPointer();
            
            variableNamePtr = generator.make_constant_string(this->getIdentifier());
            
            // get the stored object** from chime_closure_get_attribute
            value = generator.getRuntime()->callChimeClosureGetAttribute(self, variableNamePtr);
            
            alloca = generator.builder()->CreateAlloca(generator.getRuntime()->getChimeObjectPtrType(), 0, "closure attribute");
            
            objectPtrPtr = generator.builder()->CreateLoad(value, "closure attribute**");
            objectPtr    = generator.builder()->CreateLoad(objectPtrPtr, "closure attribute*");
            
            generator.builder()->CreateStore(objectPtr, alloca, false);
            
            return alloca;
        }
        
        value = generator.value_for_identifier(this->getIdentifier());
        
        if (!value)
        {
            value = generator.insertChimeObjectAlloca();
            
            generator.set_value_for_identifier(this->getIdentifier(), value);
        }
        
        return value;
    }
}
