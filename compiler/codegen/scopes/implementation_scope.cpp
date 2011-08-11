// implementation_scope.cpp

#include "implementation_scope.h"

namespace chime
{
    std::vector<std::string> ImplementationScope::getInstanceVariableNames() const
    {
        return _instanceVariableNames;
    }
    
    void ImplementationScope::addInstanceVariableName(const std::string& name)
    {
        _instanceVariableNames.push_back(name);
    }
    
    bool ImplementationScope::containsInstanceVariableName(const std::string& name)
    {
        return std::find(_instanceVariableNames.begin(), _instanceVariableNames.end(), name) != _instanceVariableNames.end();
    }
}
