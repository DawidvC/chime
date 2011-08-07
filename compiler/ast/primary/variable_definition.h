#ifndef VARIABLE_DEFINITION
#define VARIABLE_DEFINITION

#include "compiler/ast/node.h"
#include "compiler/ast/primary/type_reference.h"

namespace ast
{
    class variable_definition : public node
    {
    public:
        variable_definition(chime::parser* parser, ast::type_reference* type);
        virtual ~variable_definition();
        
        virtual std::string node_name(void) const;
        virtual std::string to_string(void) const;
        
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
