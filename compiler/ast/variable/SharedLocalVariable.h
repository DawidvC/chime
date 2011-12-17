// chime: SharedLocalVariable.h

#ifndef SHARED_LOCAL_VARIABLE_H
#define SHARED_LOCAL_VARIABLE_H

#include "Variable.h"

namespace chime
{
    class SharedLocalVariable : public Variable
    {
    public:
        SharedLocalVariable(const std::string& identifier);
        
        std::string nodeName() const;
        
        bool requiresCapture() const;
        
        ast::AssignmentOperator* createAssignment();
        Variable*                createClosedVersion();
        
        llvm::Value* codegenReference(code_generator& generator);
        llvm::Value* codegen(code_generator& generator);
    };
    
    typedef shared_ptr<SharedLocalVariable> SharedLocalVariableRef;
}

#endif // SHARED_LOCAL_VARIABLE_H
