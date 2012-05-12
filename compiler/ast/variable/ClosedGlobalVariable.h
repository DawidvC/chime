// chime: ClosedGlobalVariable.h

#ifndef CLOSED_GLOBAL_VARIABLE_H
#define CLOSED_GLOBAL_VARIABLE_H

#include "ClosedLocalVariable.h"

namespace chime
{
    class ClosedGlobalVariable : public ClosedLocalVariable
    {
    public:
        ClosedGlobalVariable(const std::string& identifier);
        
        std::string nodeName() const;
    };
    
    typedef std::tr1::shared_ptr<ClosedGlobalVariable> ClosedGlobalVariableRef;
}

#endif // CLOSED_GLOBAL_VARIABLE_H
