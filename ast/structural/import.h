#ifndef IMPORT_NODE
#define IMPORT_NODE

#include "ast/node.h"

namespace ast
{
    class import : public node
    {
    public:
        import(chime::parser* parser);
        virtual ~import();
        
        virtual std::string node_name(void) const;
        
        ast::node* importand() const;
        void       importand(ast::node* n);
    };
}

#endif
