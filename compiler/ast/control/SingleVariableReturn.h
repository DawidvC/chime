// chime: SingleVariableReturn.h

#ifndef SINGLE_VARIABLE_RETURN_H
#define SINGLE_VARIABLE_RETURN_H

#include "return.h"
#include "compiler/ast/variable/Variable.h"

namespace chime
{
    class SingleVariableReturn : public Return
    {
    public:
        std::string nodeName() const;
        
        VariableRef returnVariable() const;
        
        llvm::Value* codegen(CodeGenContext& generator);
    };
}

#endif // RETURN_H
