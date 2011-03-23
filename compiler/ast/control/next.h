#ifndef NEXT
#define NEXT

#include <string>
#include "../node.h"

namespace ast
{
    class next : public node
    {
    public:
        next(chime::parser* parser);
        
        virtual std::string node_name(void);
    };
}
#endif
