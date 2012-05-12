// chime: Control.h

#ifndef CONTROL_H
#define CONTROL_H

#include "compiler/ast/node.h"

namespace chime
{
    class Control : public Node
    {
    public:
        static Node* parse(chime::parser& parser);
    };
}

#endif // CONTROL_H
