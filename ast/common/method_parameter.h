#ifndef METHOD_PARAMETER
#define METHOD_PARAMETER

#include "ast/node.h"
#include "ast/primary/type_reference.h"

namespace ast
{
    class method_parameter : public node
    {
    public:
        method_parameter(chime::parser* parser);
        virtual ~method_parameter();
        
        virtual std::string node_name(void) const;
        virtual std::string to_string(void) const;
        
        std::string          identifier() const;
        void                 identifier(std::string s);
        std::string          label() const;
        void                 label(std::string s);
        ast::type_reference* type() const;
        void                 type(ast::type_reference* t);
        
    private:
        std::string          _identifier;
        std::string          _label;
        ast::type_reference* _type;
    };
}

#endif