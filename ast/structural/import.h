#include <string>
#include "../node.h"

#ifndef IMPORT_NODE
#define IMPORT_NODE

namespace ast
{
    class import : public node
    {
    public:
        import(chime::parser* parser);
        virtual ~import();
        
        virtual std::string node_name(void);
        
        ast::node* importand() const;
        void       importand(ast::node* n);
    };
}

#endif
