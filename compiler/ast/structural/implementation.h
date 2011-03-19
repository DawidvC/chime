#include <string>
#include "../node.h"
#include "../primary/type_reference.h"

#ifndef IMPLEMENTATION_NODE
#define IMPLEMENTATION_NODE

namespace ast
{
    class implementation : public node
    {
    public:
        implementation(chime::parser* parser);
        
        virtual std::string node_name(void);
        
        ast::type_reference* type_ref() const;
        void                 type_ref(ast::type_reference* n);
        ast::type_reference* super_class() const;
        void                 super_class(ast::type_reference* n);
    };
}

#endif
