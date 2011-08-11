// implementation_scope.h

#ifndef IMPLEMENTATION_SCOPE_H
#define IMPLEMENTATION_SCOPE_H

#include "compiler/codegen/scopes/scope.h"

namespace chime
{
    class ImplementationScope : public Scope
    {
    public:
        std::vector<std::string> getInstanceVariableNames() const;
        void                     addInstanceVariableName(const std::string& name);
        bool                     containsInstanceVariableName(const std::string& name);
    
    private:
        std::vector<std::string> _instanceVariableNames;
    };
    
    typedef std::tr1::shared_ptr<chime::ImplementationScope> ImplementationScopeRef;
}

#endif // IMPLEMENTATION_SCOPE_H
