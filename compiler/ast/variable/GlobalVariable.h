// chime: GlobalVariable.h

#ifndef GLOBAL_VARIABLE_H
#define GLOBAL_VARIABLE_H

#include "Variable.h"

namespace ast
{
    class GlobalVariable : public Variable
    {
    public:
        GlobalVariable(const std::string& identifier);
        
        std::string nodeName(void) const;
        
        llvm::Value* codegen(chime::code_generator& generator);
    };
}

#endif // GLOBAL_VARIABLE_H
