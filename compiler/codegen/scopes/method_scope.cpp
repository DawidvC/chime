// method_scope.cpp

#include "method_scope.h"

namespace chime
{
    MethodScope::MethodScope()
    {
        _selfPointer        = NULL;
        _environmentPointer = NULL;
    }
    
    llvm::Value* MethodScope::getSelfPointer(void) const
    {
        return _selfPointer;
    }
    void MethodScope::setSelfPointer(llvm::Value* value)
    {
        _selfPointer = value;
    }
    
    llvm::Value* MethodScope::getEnvironmentPointer(void) const
    {
        return _environmentPointer;
    }
    void MethodScope::setEnvironmentPointer(llvm::Value* value)
    {
        _environmentPointer = value;
    }
}