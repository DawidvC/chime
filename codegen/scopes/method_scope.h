// method_scope.h

#ifndef METHOD_SCOPE_H
#define METHOD_SCOPE_H

#include "codegen/scopes/scope.h"

namespace chime
{
    class MethodScope : public Scope
    {
    public:
        llvm::BasicBlock* getReturnBlock(void) const;
        void              setReturnBlock(llvm::BasicBlock* block);
    
    protected:
        llvm::BasicBlock* _returnBlock;
    };
    
    typedef std::tr1::shared_ptr<chime::MethodScope> MethodScopeRef;
}

#endif // METHOD_SCOPE_H
