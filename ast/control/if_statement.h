#ifndef IF_STATEMENT
#define IF_STATEMENT

#include <string>
#include "ast/node.h"

namespace ast
{
    class if_statement : public node
    {
    public:
        if_statement(chime::parser* parser, ast::node* body);
        
        virtual std::string node_name(void);
        
        ast::node* condition(void) const;
        void       condition(ast::node* node);
        ast::node* body(void) const;
        void       body(ast::node* node);
    };
}
#endif
