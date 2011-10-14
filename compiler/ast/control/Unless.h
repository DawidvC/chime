// chime: Unless.h

#ifndef UNLESS_H
#define UNLESS_H

#include "Conditional.h"

namespace chime
{
    class Unless : public Conditional
    {
    public:
        virtual std::string nodeName(void) const;
        
        llvm::Value* codegen(CodeGenContext& context);
    };
}
#endif // UNLESS_H
