#ifndef OPERATOR_NODE
#define OPERATOR_NODE

#include "compiler/ast/node.h"

namespace ast
{
    class basic_operator : public node
    {
    public:
        basic_operator();
        virtual ~basic_operator();
        
        std::string identifier() const;
        void        identifier(std::string s);
        
        std::string getIdentifier() const;
        void        setIdentifier(const std::string& s);
        
    protected:
        std::string _identifier;
    };
}

namespace chime
{
    typedef ast::basic_operator Operator;
}

#endif
