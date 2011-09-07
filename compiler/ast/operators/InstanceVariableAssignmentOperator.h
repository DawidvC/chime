// chime: InstanceVariableAssignmentOperator.h

#ifndef INSTANCE_VARIABLE_ASSIGNMENT_OPERATOR_H
#define INSTANCE_VARIABLE_ASSIGNMENT_OPERATOR_H

#include "AssignmentOperator.h"

namespace ast
{
    class InstanceVariableAssignmentOperator : public AssignmentOperator
    {
    public:
        std::string nodeName(void) const;
    };
}

#endif // INSTANCE_VARIABLE_ASSIGNMENT_OPERATOR_H
