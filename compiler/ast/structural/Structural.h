// chime: Structural.h

#ifndef STRUCTURAL_H
#define STRUCTURAL_H

#include "compiler/ast/node.h"

namespace ast
{
    class Structural : public node
    {
    public:
        static ast::node* parse(chime::parser& parser);
    };
}

#endif // STRUCTURAL_H
