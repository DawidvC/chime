#include <string>
#include <vector>
#include "../node.h"

#ifndef IMPORT_NODE
#define IMPORT_NODE

namespace ast
{
    class import : public node
    {
    public:
        import(chime::parser* parser);
        
        virtual std::string node_name(void);
        
        ast::node* expression() const;
        void       expression(ast::node* n);
    };
}

#endif
