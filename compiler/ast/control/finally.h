// chime: finally.h

#ifndef FINALLY_H
#define FINALLY_H

#include "exception_flow.h"

namespace ast
{
    class Finally : public ExceptionFlow
    {
    public:
        Finally(chime::parser& parser);
        
        virtual std::string nodeName(void) const;
    };
}

#endif // FINALLY_H
