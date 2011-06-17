#ifndef NEXT
#define NEXT

#include "ast/node.h"

namespace ast
{
    class next : public node
    {
    public:
        next(chime::parser* parser);
        
        virtual std::string node_name(void) const;
    };
}
#endif
