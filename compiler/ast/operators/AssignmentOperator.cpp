#include "AssignmentOperator.h"

namespace chime
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
        
        // if this is a variable, we cannot rely on the +1 guarantee, and we
        // need to retain explicitly
        if (this->getRightOperand()->isVariable() || this->getRightOperand()->isSelf())
        {
            generator.getRuntime()->callChimeObjectRetain(rValue);
        }
        
        generator.getCurrentScope()->removeLooseValue(rValue);
        
        objectLoad = generator.builder()->CreateLoad(rValue, "assignment rValue");
        
        // Assignments can also "create" variables.  If needed, we do that here
        variable = dynamic_cast<chime::Variable*>(this->getLeftOperand());
        
        lValue = variable->codegen(generator);
        assert(lValue);
        
        //fprintf(stderr, "name:%s defined:%d\n", variable->getIdentifier().c_str(), variable->isDefined());
        // if the variable being assigned to is defined, we have to release what
        // it was pointing to
        if (variable->isDefined())
        {
            // here, we need to decrement the retain count first!
            generator.getRuntime()->callChimeObjectRelease(lValue);
        }
        
        generator.builder()->CreateStore(objectLoad, lValue, false);
        
        return lValue;
    }
}
