// chime: ClosedLocalVariableAssignmentOperator.h

#ifndef CLOSED_LOCAL_VARIABLE_ASSIGNMENT_OPERATOR_H
#define CLOSED_LOCAL_VARIABLE_ASSIGNMENT_OPERATOR_H

#include "AssignmentOperator.h"

namespace chime
{
    class ClosedLocalVariableAssignmentOperator : public AssignmentOperator
    {
    public:
        std::string nodeName(void) const;
        
        llvm::Value* codegen(chime::code_generator& generator);
    };
}

#endif // CLOSED_LOCAL_VARIABLE_ASSIGNMENT_OPERATOR_H
