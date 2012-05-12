// chime: InterpolatedString.h

#ifndef INTERPOLATED_STRING_H
#define INTERPOLATED_STRING_H

#include "compiler/ast/node.h"

namespace chime
{
    class InterpolatedString : public Node
    {
    public:
        static Node* parse(chime::parser& parser);
        
    public:
        std::string nodeName(void) const;
    };
}

#endif // INTERPOLATED_STRING_H
