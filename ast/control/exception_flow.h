// chime: exception_flow.h

#ifndef EXCEPTION_FLOW_H
#define EXCEPTION_FLOW_H

#include "ast/node.h"

namespace ast
{
    class ExceptionFlow : public node
    {
    public:
        virtual std::string nodeName(void) = 0;
        std::string node_name(void);
        
    protected:
        void parseBody(chime::parser& parser);
    };
}

#endif // EXCEPTION_FLOW_H
