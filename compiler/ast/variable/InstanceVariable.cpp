#include "InstanceVariable.h"

namespace ast
{
    InstanceVariable::InstanceVariable(const std::string& identifier) :
        Variable(identifier)
    {
    }
    
    std::string InstanceVariable::nodeName(void) const
    {
        if (this->getClosed())
            return std::string("Closed Instance Variable");
        else
            return std::string("Instance Variable");
    }
}
