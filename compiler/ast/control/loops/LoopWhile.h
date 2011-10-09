// chime: LoopWhile.h

#ifndef LOOP_WHILE_H
#define LOOP_WHILE_H

#include "compiler/ast/control/Loop.h"

namespace chime
{
    class LoopWhile : public Loop
    {
    public:
        std::string nodeName(void) const;
    };
}
#endif // LOOP_WHILE_H
