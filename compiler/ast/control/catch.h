// chime: catch.h

#ifndef CATCH_H
#define CATCH_H

#include "exception_flow.h"
#include "compiler/ast/primary/type_reference.h"
#include "compiler/ast/primary/entity_reference.h"

namespace ast
{
    class Catch : public ExceptionFlow
    {
    public:
        Catch(chime::parser& parser);
        virtual ~Catch();
        
        virtual std::string nodeName(void) const;
    
    protected:
        ast::entity_reference* _exceptionVariable;
        ast::type_reference*   _exceptionType;
    };
}

#endif // CATCH_H