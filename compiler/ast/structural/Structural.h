// chime: Structural.h

#ifndef STRUCTURAL_H
#define STRUCTURAL_H

#include "compiler/ast/ScopedNode.h"

namespace chime
{
    class Structural : public ScopedNode
    {
    public:
        static Node* parse(chime::parser& parser);
    };
}

#endif // STRUCTURAL_H
