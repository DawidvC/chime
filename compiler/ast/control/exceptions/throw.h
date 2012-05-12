// chime: throw.h

#ifndef THROW_H
#define THROW_H

#include "compiler/ast/node.h"

namespace chime
{
    class Throw : public node
    {
    public:
        Throw(chime::parser& parser);
        
        virtual std::string nodeName(void) const;
    };
}

#endif // THROW_H
