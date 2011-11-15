// chime: AssignmentOperator.h

#ifndef ASSIGNMENT_OPERATOR_H
#define ASSIGNMENT_OPERATOR_H

#include "binary_operator.h"

namespace chime
{
    class Variable;
}

namespace ast
{
    class AssignmentOperator : public binary_operator
    {
    public:
        chime::Variable* getVariable() const;
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
    };
}

#endif // ASSIGNMENT_OPERATOR_H
