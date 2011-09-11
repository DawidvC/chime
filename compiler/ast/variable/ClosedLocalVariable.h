// chime: ClosedLocalVariable.h

#ifndef CLOSED_LOCAL_VARIABLE_H
#define CLOSED_LOCAL_VARIABLE_H

#include "Variable.h"

namespace ast
{
    class ClosedLocalVariable : public Variable
    {
    public:
        ClosedLocalVariable(const std::string& identifier);
        
        std::string nodeName(void) const;
        
        AssignmentOperator* createAssignment();
        
        llvm::Value* codegen(chime::code_generator& generator);
    };
    
    typedef std::tr1::shared_ptr<ClosedLocalVariable> ClosedLocalVariableRef;
}

#endif // CLOSED_LOCAL_VARIABLE_H