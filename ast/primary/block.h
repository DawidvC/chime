#include <string>
#include "../node.h"
#include "../common/code_container.h"

#ifndef BLOCK
#define BLOCK

namespace ast
{
    class block : public code_container
    {
    public:
        block(chime::parser* parser);
        
        virtual std::string node_name(void);
    };
}

#endif
