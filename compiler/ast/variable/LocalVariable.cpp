#include "LocalVariable.h"

namespace ast
{
    LocalVariable::LocalVariable(const std::string& identifier) :
        Variable(identifier)
    {
    }
    
    std::string LocalVariable::nodeName(void) const
    {
        if (this->getClosed())
            return std::string("Closed Local Variable");
        else
            return std::string("Local Variable");
    }
}
