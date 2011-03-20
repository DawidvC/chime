#include <string>
#include "../node.h"
#include "../primary/type_reference.h"

#ifndef VARIABLE_DEFINITION
#define VARIABLE_DEFINITION

namespace ast
{
    class variable_definition : public node
    {
    public:
        variable_definition(chime::parser* parser);
        virtual ~variable_definition();
        
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
