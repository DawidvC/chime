// chime: Structural.h

#ifndef STRUCTURAL_H
#define STRUCTURAL_H

#include "compiler/ast/ScopedNode.h"

namespace ast
{
    class Structural : public ScopedNode
    {
    public:
        static ast::node* parse(chime::parser& parser);
    };
}

#endif // STRUCTURAL_H
