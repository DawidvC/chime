// chime: Until.h

#ifndef UNTIL_H
#define UNTIL_H

#include "compiler/ast/control/Loop.h"

namespace chime
{
    class Until : public Loop
    {
    public:
        static Until* parse(chime::parser& parser);
        
    public:
        std::string nodeName(void) const;
        
        llvm::Value* codegen(chime::code_generator& generator);
    };
}
#endif // UNTIL_H
