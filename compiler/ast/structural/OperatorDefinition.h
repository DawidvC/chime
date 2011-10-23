// chime: OperatorDefinition.h

#ifndef OPERATOR_DEFINITON_H
#define OPERATOR_DEFINITON_H

#include "method_definition.h"

namespace chime
{
    class OperatorDefinition : public ast::method_definition
    {
    public:
        static OperatorDefinition* parse(parser& parser);
    
    public:
        virtual std::string nodeName() const;
    };
}

#endif // OPERATOR_DEFINITON_H
