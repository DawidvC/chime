#include <string>
#include "../node.h"

#ifndef ENTITY_REFERENCE_NODE
#define ENTITY_REFERENCE_NODE

namespace ast
{
    class entity_reference : public node
    {
    public:
        entity_reference();
        entity_reference(chime::parser* parser);
        virtual ~entity_reference();
        
        virtual std::string node_name(void);
        virtual std::string to_string(void);
        
        virtual std::string identifier() const;
        virtual void identifier(std::string s);
        
    protected:
        std::string _identifier;
    };
}

#endif
