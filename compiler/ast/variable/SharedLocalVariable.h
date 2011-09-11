// chime: SharedLocalVariable.h

#ifndef SHARED_LOCAL_VARIABLE_H
#define SHARED_LOCAL_VARIABLE_H

#include "Variable.h"

namespace ast
{
    class SharedLocalVariable : public Variable
    {
    public:
        SharedLocalVariable(const std::string& identifier);
        
        std::string nodeName(void) const;
        
        // AssignmentOperator* createAssignment();
        
        llvm::Value* codegen(chime::code_generator& generator);
    };
    
    typedef std::tr1::shared_ptr<SharedLocalVariable> SharedLocalVariableRef;
}

#endif // SHARED_LOCAL_VARIABLE_H
