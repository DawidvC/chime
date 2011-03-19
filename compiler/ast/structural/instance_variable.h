#include <string>
#include "../node.h"
#include "../primary/type_reference.h"

#ifndef INSTANCE_VARIABLE
#define INSTANCE_VARIABLE

namespace ast
{
    class instance_variable : public node
    {
    public:
        instance_variable(chime::parser* parser);
        virtual ~instance_variable();
        
        virtual std::string node_name(void);
        virtual std::string to_string(void);
        
        ast::type_reference* variable_type() const;
        void                 variable_type(ast::type_reference* n);
        std::string          identifier() const;
        void                 identifier(std::string s);
        
    protected:
        ast::type_reference* _type_ref;
        std::string          _identifier;
    };
}

#endif
