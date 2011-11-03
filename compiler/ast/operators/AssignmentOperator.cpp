#include "AssignmentOperator.h"

namespace ast
{
    Variable* AssignmentOperator::getVariable() const
    {
        return static_cast<Variable*>(this->getLeftOperand());
    }
    
    llvm::Value* AssignmentOperator::codegen(chime::code_generator& generator)
    {
        ast::Variable*  variable;
        llvm::Value*    lValue;
        llvm::Value*    rValue;
        llvm::LoadInst* objectLoad;
        
        // first, codegen the thing we're going to be assigning to the variable
        rValue = this->getRightOperand()->codegen(generator);
        assert(rValue);
        
        objectLoad = generator.builder()->CreateLoad(rValue, "assignment rValue");
        
        // Assignments can also "create" variables.  If needed, we do that here
        variable = static_cast<ast::Variable*>(this->getLeftOperand());
        
        lValue = variable->codegen(generator);
        //lValue = generator.getCurrentScope()->getValueForIdentifier(variable->getIdentifier());
        //if (!lValue)
        //{
        //    
        //    lValue = generator.insertChimeObjectAlloca();
        //    
        //    generator.getCurrentScope()->setValueForIdentifier(variable->getIdentifier(), lValue);
        //}
        
        assert(lValue);
        
        generator.builder()->CreateStore(objectLoad, lValue, false);
        
        return lValue;
    }
}
