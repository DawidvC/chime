#ifndef BLOCK
#define BLOCK

#include "compiler/ast/common/code_container.h"

namespace ast
{
    class block : public code_container
    {
    public:
        block(chime::parser* parser);
        
        virtual std::string node_name(void) const;
    };
}

#endif
