#include "ClosedGlobalVariable.h"

namespace chime
{
    ClosedGlobalVariable::ClosedGlobalVariable(const std::string& identifier) :
        ClosedLocalVariable(identifier)
    {
    }
    
    std::string ClosedGlobalVariable::nodeName() const
    {
        return "Closed Global Variable";
    }
}
