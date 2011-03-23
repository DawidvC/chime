#ifndef STRING_LITERAL
#define STRING_LITERAL

#include <string>
#include "../node.h"

namespace ast
{
    class string_literal : public node
    {
    public:
        string_literal(chime::parser* parser);
        
        virtual std::string node_name(void);
        virtual std::string to_string(void);
        
        std::string value(void) const;
        void value(std::string v);
        
    private:
        std::string _value;
    };
}

#endif
