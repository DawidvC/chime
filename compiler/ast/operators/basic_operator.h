#ifndef OPERATOR_NODE
#define OPERATOR_NODE

#include "compiler/ast/node.h"

namespace chime
{
    class basic_operator : public node
    {
    public:
        std::string identifier() const;
        void        identifier(std::string s);
        
        std::string getIdentifier() const;
        void        setIdentifier(const std::string& s);
        
    protected:
        std::string _identifier;
    };
}

#endif
