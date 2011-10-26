// chime:: ClosedSelfLiteral.h

#ifndef CLOSED_SELF_LITERAL_H
#define CLOSED_SELF_LITERAL_H

#include "SelfLiteral.h"

namespace chime
{
    class ClosedSelfLiteral : public SelfLiteral
    {
    public:
        virtual std::string nodeName() const;
    };
}

#endif // CLOSED_SELF_LITERAL_H
