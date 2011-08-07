// chime: exception_flow.h

#ifndef EXCEPTION_FLOW_H
#define EXCEPTION_FLOW_H

#include "compiler/ast/node.h"

namespace ast
{
    class ExceptionFlow : public node
    {
    protected:
        void parseBody(chime::parser& parser);
    };
}

#endif // EXCEPTION_FLOW_H
