// method_scope.h

#ifndef METHOD_SCOPE_H
#define METHOD_SCOPE_H

#include "codegen/scopes/scope.h"

namespace chime
{
    class MethodScope : public Scope
    {
    public:
        MethodScope();
        virtual ~MethodScope();
    };
}

#endif // METHOD_SCOPE_H
