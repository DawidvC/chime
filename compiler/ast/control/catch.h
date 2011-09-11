// chime: catch.h

#ifndef CATCH_H
#define CATCH_H

#include "exception_flow.h"
#include "compiler/ast/primary/type_reference.h"
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
        ast::Variable*       _exceptionVariable;
        ast::type_reference* _exceptionType;
    };
}

#endif // CATCH_H
