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
        virtual ~implementation();
        
        virtual std::string node_name(void);
        virtual std::string to_string(void);
        
        ast::type_reference* type_ref() const;
        void                 type_ref(ast::type_reference* node);
        ast::type_reference* super_class() const;
        void                 super_class(ast::type_reference* node);
        
    protected:
        ast::type_reference* _type_ref;
        ast::type_reference* _super_class;
        
        void parse_body(chime::parser* parser);
    };
}

#endif
