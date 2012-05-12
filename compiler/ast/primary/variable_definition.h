#ifndef VARIABLE_DEFINITION
#define VARIABLE_DEFINITION

#include "compiler/ast/node.h"
#include "compiler/ast/structural/Type.h"

namespace chime
{
    class variable_definition : public node
    {
    public:
        variable_definition(chime::parser* parser, chime::Type* type);
        virtual ~variable_definition();
        
        virtual std::string node_name(void) const;
        virtual std::string to_string(void) const;
        
        chime::Type* variable_type() const;
        void         variable_type(chime::Type* n);
        std::string  identifier() const;
        void         identifier(std::string s);
        
    protected:
        chime::Type* _type_ref;
        std::string  _identifier;
    };
}

#endif
