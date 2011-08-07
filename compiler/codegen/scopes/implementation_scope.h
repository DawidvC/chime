// implementation_scope.h

#ifndef IMPLEMENTATION_SCOPE_H
#define IMPLEMENTATION_SCOPE_H

#include "compiler/codegen/scopes/scope.h"

namespace chime
{
    class ImplementationScope : public Scope
    {
    public:
        void placeholder(void); // just here to remove a warning
    };
    
    typedef std::tr1::shared_ptr<chime::ImplementationScope> ImplementationScopeRef;
}

#endif // IMPLEMENTATION_SCOPE_H
