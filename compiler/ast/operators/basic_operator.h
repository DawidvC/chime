#include <string>
#include "../node.h"

#ifndef OPERATOR_NODE
#define OPERATOR_NODE

namespace ast
{
    class basic_operator : public node
    {
    public:
        basic_operator();
        
        std::string identifier() const;
        void identifier(std::string s);
        
    protected:
        std::string _identifier;
    };
}

#endif
