// chime: InstanceVariable.h

#ifndef INSTANCE_VARIABLE_H
#define INSTANCE_VARIABLE_H

#include "Variable.h"

namespace ast
{
    class InstanceVariable : public chime::Variable
    {
    public:
        InstanceVariable(const std::string& identifier);
        
        std::string nodeName(void) const;
        
        AssignmentOperator* createAssignment();
        
        llvm::Value* codegen(chime::code_generator& generator);
    };
    
    typedef std::tr1::shared_ptr<InstanceVariable> InstanceVariableRef;
}

#endif // INSTANCE_VARIABLE_H
