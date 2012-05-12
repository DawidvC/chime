// chime: LocalVariable.h

#ifndef LOCAL_VARIABLE_H
#define LOCAL_VARIABLE_H

#include "Variable.h"

namespace chime
{
    class LocalVariable : public chime::Variable
    {
    public:
        LocalVariable(const std::string& identifier);
        
        virtual std::string nodeName() const;
        
        bool requiresCapture() const;
        
        AssignmentOperator* createAssignment();
        chime::Variable*    createClosedVersion();
        
        llvm::Value* codegen(chime::code_generator& generator);
    };
    
    typedef shared_ptr<LocalVariable> LocalVariableRef;
}

#endif // LOCAL_VARIABLE_H
