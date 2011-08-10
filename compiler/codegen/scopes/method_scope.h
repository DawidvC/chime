// method_scope.h

#ifndef METHOD_SCOPE_H
#define METHOD_SCOPE_H

#include "compiler/codegen/scopes/scope.h"

namespace chime
{
    class MethodScope : public Scope
    {
    public:
        llvm::Value*      getSelfPointer(void) const;
        void              setSelfPointer(llvm::Value* value);
    
    protected:
        llvm::Value*      _selfPointer;
    };
    
    typedef std::tr1::shared_ptr<chime::MethodScope> MethodScopeRef;
}

#endif // METHOD_SCOPE_H
