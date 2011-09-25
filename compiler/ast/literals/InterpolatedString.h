// chime: InterpolatedString.h

#ifndef INTERPOLATED_STRING_H
#define INTERPOLATED_STRING_H

#include "compiler/ast/node.h"

namespace ast
{
    class InterpolatedString : public Node
    {
    public:
        static ast::Node* parse(chime::parser& parser);
        
    public:
        std::string nodeName(void) const;
    };
}

#endif // INTERPOLATED_STRING_H
