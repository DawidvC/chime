#include "AssignmentOperator.h"

namespace ast
{
    chime::Variable* AssignmentOperator::getVariable() const
    {
        return dynamic_cast<chime::Variable*>(this->getLeftOperand());
    }
    
    llvm::Value* AssignmentOperator::codegen(chime::code_generator& generator)
    {
        chime::Variable*  variable;
        llvm::Value*      lValue;
        llvm::Value*      rValue;
        llvm::LoadInst*   objectLoad;
        
        // first, codegen the thing we're going to be assigning to the variable
        rValue = this->getRightOperand()->codegen(generator);
        assert(rValue);
        
        generator.getCurrentScope()->removeLooseValue(rValue);
        
        objectLoad = generator.builder()->CreateLoad(rValue, "assignment rValue");
        
        // Assignments can also "create" variables.  If needed, we do that here
        variable = dynamic_cast<chime::Variable*>(this->getLeftOperand());
        
        lValue = variable->codegen(generator);
        assert(lValue);
        
        // if the variable being assigned to is defined, we have to release what
        // it was pointing to, and then retain the new value.  But!  If, the variable
        // is being defined for the first time AND the thing we are assigning it to is
        // not itself a variable, then we can rely on the +1 garantee
        if (variable->isDefined())
        {
            fprintf(stderr, "defined: %s\n", variable->getIdentifier().c_str());
            // here, we need to decrement the retain count first!
            generator.getRuntime()->callChimeObjectRelease(lValue);
        }
        
        if (this->getRightOperand()->isVariable())
        {
            generator.getRuntime()->callChimeObjectRetain(rValue);
        }
        
        generator.builder()->CreateStore(objectLoad, lValue, false);
        
        return lValue;
    }
}
