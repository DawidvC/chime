#ifndef METHOD_DEFINITON
#define METHOD_DEFINITON

#include <string>
#include "../node.h"
#include "../common/code_container.h"

namespace ast
{
    class method_definition : public code_container
    {
    public:
        method_definition(chime::parser* parser);
        
        virtual std::string node_name(void);
        virtual std::string to_string(void);
        
        std::string         identifier() const;
        void                identifier(std::string s);
        
    protected:
        std::string                     _identifier;
    };
}

#endif