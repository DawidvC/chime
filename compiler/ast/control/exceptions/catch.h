// chime: catch.h

#ifndef CATCH_H
#define CATCH_H

#include "compiler/ast/control/exception_flow.h"
#include "compiler/ast/structural/Type.h"
#include "compiler/ast/variable/Variable.h"

namespace ast
{
    class Catch : public ExceptionFlow
    {
    public:
        Catch(chime::parser& parser);
        virtual ~Catch();
        
        virtual std::string nodeName(void) const;
    
    protected:
        chime::Variable* _exceptionVariable;
        chime::Type*     _exceptionType;
    };
}

#endif // CATCH_H
