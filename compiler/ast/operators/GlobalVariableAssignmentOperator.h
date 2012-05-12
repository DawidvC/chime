// chime: GlobalVariableAssignmentOperator.h

#ifndef GLOBAL_VARIABLE_ASSIGNMENT_OPERATOR_H
#define GLOBAL_VARIABLE_ASSIGNMENT_OPERATOR_H

#include "AssignmentOperator.h"

namespace chime
{
    class GlobalVariableAssignmentOperator : public AssignmentOperator
    {
    public:
        std::string nodeName(void) const;
    };
}

#endif // GLOBAL_VARIABLE_ASSIGNMENT_OPERATOR_H
