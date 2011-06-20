// implementation_scope.h

#ifndef IMPLEMENTATION_SCOPE_H
#define IMPLEMENTATION_SCOPE_H

#include "codegen/scopes/scope.h"

namespace chime
{
    class ImplementationScope : public Scope
    {
    };
    
    typedef std::tr1::shared_ptr<chime::ImplementationScope> ImplementationScopeRef;
}

#endif // IMPLEMENTATION_SCOPE_H
