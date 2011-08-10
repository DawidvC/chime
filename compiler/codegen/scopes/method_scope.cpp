// method_scope.cpp

#include "method_scope.h"

namespace chime
{
    llvm::Value* MethodScope::getSelfPointer(void) const
    {
        return _selfPointer;
    }
    void MethodScope::setSelfPointer(llvm::Value* value)
    {
        _selfPointer = value;
    }
}