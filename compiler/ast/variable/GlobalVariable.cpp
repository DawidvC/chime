#include "GlobalVariable.h"

namespace ast
{
    GlobalVariable::GlobalVariable(const std::string& identifier) :
        Variable(identifier)
    {
    }
    
    std::string GlobalVariable::nodeName(void) const
    {
        if (this->getClosed())
            return std::string("Closed Global Variable");
        else
            return std::string("Global Variable");
    }
}