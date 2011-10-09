// chime: LoopUntil.h

#ifndef LOOP_UNTIL_H
#define LOOP_UNTIL_H

#include "compiler/ast/control/Loop.h"

namespace chime
{
    class LoopUntil : public Loop
    {
    public:
        std::string nodeName(void) const;
    };
}
#endif // LOOP_UNTIL_H
