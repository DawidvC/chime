#include <string>
#include "../node.h"

#ifndef ENTITY_REFERENCE_NODE
#define ENTITY_REFERENCE_NODE

namespace ast
{
    class entity_reference : public node
    {
    public:
        entity_reference(chime::parser* parser);
        
        virtual std::string node_name(void);
        virtual std::string to_string(void);
        
        std::string identifier() const;
        void identifier(std::string s);
        
    protected:
        std::string _identifier;
    };
}

#endif
