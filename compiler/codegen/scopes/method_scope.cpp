// method_scope.cpp

#include "method_scope.h"

namespace chime
{
    llvm::BasicBlock* MethodScope::getReturnBlock(void) const
    {
        return _returnBlock;
    }
    void MethodScope::setReturnBlock(llvm::BasicBlock* block)
    {
        _returnBlock = block;
    }
}