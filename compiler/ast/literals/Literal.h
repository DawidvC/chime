#ifndef LITERAL_H
#define LITERAL_H

#include "compiler/ast/node.h"

namespace ast
{
    class Literal : public node
    {
    public:
        static ast::Literal* parse(chime::parser& parser);
    };
}

#endif
