#include <string>
#include "../node.h"

#ifndef IMPLEMENTATION_NODE
#define IMPLEMENTATION_NODE

namespace ast
{
    class implementation : public node
    {
    public:
        implementation(chime::parser* parser);
        
        virtual std::string node_name(void);
    };
}

#endif
