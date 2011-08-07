#ifndef NEXT
#define NEXT

#include "compiler/ast/node.h"

namespace ast
{
    class next : public node
    {
    public:
        next(chime::parser* parser);
        
        virtual std::string nodeName(void) const;
    };
}
#endif
