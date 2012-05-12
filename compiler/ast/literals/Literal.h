#ifndef LITERAL_H
#define LITERAL_H

#include "compiler/ast/node.h"

namespace chime
{
    class Literal : public node
    {
    public:
        static Node* parse(chime::parser& parser);
    };
}

#endif
