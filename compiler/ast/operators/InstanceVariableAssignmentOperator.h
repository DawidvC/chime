// chime: InstanceVariableAssignmentOperator.h

#ifndef INSTANCE_VARIABLE_ASSIGNMENT_OPERATOR_H
#define INSTANCE_VARIABLE_ASSIGNMENT_OPERATOR_H

#include "AssignmentOperator.h"

namespace chime
{
    class InstanceVariableAssignmentOperator : public AssignmentOperator
    {
    public:
        std::string nodeName(void) const;
        
        llvm::Value* codegen(chime::code_generator& generator);
    };
}

#endif // INSTANCE_VARIABLE_ASSIGNMENT_OPERATOR_H
