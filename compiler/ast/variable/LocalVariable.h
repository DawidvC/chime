// chime: LocalVariable.h

#ifndef LOCAL_VARIABLE_H
#define LOCAL_VARIABLE_H

#include "Variable.h"

namespace ast
{
    class LocalVariable : public chime::Variable
    {
    public:
        LocalVariable(const std::string& identifier);
        
        virtual std::string nodeName() const;
        
        AssignmentOperator* createAssignment();
        
        llvm::Value* codegen(chime::code_generator& generator);
    };
    
    typedef std::tr1::shared_ptr<LocalVariable> LocalVariableRef;
}

#endif // LOCAL_VARIABLE_H
