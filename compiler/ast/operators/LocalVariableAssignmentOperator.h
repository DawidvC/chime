// chime: LocalVariableAssignmentOperator.h

#ifndef LOCAL_VARIABLE_ASSIGNMENT_OPERATOR_H
#define LOCAL_VARIABLE_ASSIGNMENT_OPERATOR_H

#include "AssignmentOperator.h"

namespace ast
{
    class LocalVariableAssignmentOperator : public AssignmentOperator
    {
    public:
        std::string nodeName(void) const;
    };
}

#endif // LOCAL_VARIABLE_ASSIGNMENT_OPERATOR_H
