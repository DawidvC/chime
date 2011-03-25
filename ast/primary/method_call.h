#include <string>
#include "../node.h"

#ifndef METHOD_CALL
#define METHOD_CALL

namespace ast
{
    class method_call : public node
    {
    public:
        method_call(chime::parser* parser);
        
        virtual std::string node_name(void);
        virtual std::string to_string(void);
        
        std::string identifier() const;
        void identifier(std::string s);
        
    protected:
        std::string _identifier;
        
        void parse_arguments(chime::parser* parser);
    };
}

#endif