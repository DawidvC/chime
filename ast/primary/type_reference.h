#include <string>
#include "../node.h"

#ifndef TYPE_REFERENCE
#define TYPE_REFERENCE

namespace ast
{
    class type_reference : public node
    {
    public:
        type_reference(chime::parser* parser);
        
        virtual std::string node_name(void);
        virtual std::string to_string(void);
        
        std::string identifier() const;
        void identifier(std::string s);
        
    protected:
        std::string _identifier;
        
        void parse_parameters(chime::parser* parser);
    };
}

#endif
