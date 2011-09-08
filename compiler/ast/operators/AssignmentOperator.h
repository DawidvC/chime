// chime: AssignmentOperator.h

#ifndef ASSIGNMENT_OPERATOR_H
#define ASSIGNMENT_OPERATOR_H

#include "binary_operator.h"

namespace ast
{
    class Variable;
}

namespace ast
{
    class AssignmentOperator : public binary_operator
    {
    public:
        Variable* getVariable() const;
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
    };
}

#endif // ASSIGNMENT_OPERATOR_H
