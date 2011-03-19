#include <string>
#include "../node.h"
#include "../primary/type_reference.h"

#ifndef IMPORT_NODE
#define IMPORT_NODE

namespace ast
{
    class import : public node
    {
    public:
        import(chime::parser* parser);
        
        virtual std::string node_name(void);
        
        ast::type_reference* importand() const;
        void                 importand(ast::type_reference* n);
    };
}

#endif
