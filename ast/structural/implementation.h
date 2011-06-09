#ifndef IMPLEMENTATION_NODE
#define IMPLEMENTATION_NODE

#include "ast/common/code_container.h"
#include "ast/primary/type_reference.h"

namespace ast
{
    class implementation : public code_container
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
    };
}

#endif
