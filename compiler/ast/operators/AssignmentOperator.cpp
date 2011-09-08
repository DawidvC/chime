#include "AssignmentOperator.h"

namespace ast
{
    Variable* AssignmentOperator::getVariable() const
    {
        return static_cast<Variable*>(this->getLeftOperand());
    }
    
    llvm::Value* AssignmentOperator::codegen(chime::code_generator& generator)
    {
        llvm::Value*    lValue;
        llvm::Value*    rValue;
        llvm::LoadInst* objectLoad;
        
        // first, codegen the thing we're going to be assigning to the variable
        rValue = this->getRightOperand()->codegen(generator);
        assert(rValue);
        
        objectLoad = generator.builder()->CreateLoad(rValue, "loaded r_value");
        
        // this is just a regular old local variable
        lValue = this->getLeftOperand()->codegen(generator);
        assert(lValue);
        
        generator.builder()->CreateStore(objectLoad, lValue, false);
        
        return lValue;
    }
}
