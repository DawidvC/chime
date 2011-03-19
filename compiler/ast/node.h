#include <vector>
#include "../parser/parser.h"

#ifndef AST_NODE
#define AST_NODE

namespace ast
{
    class node
    {
    public:
        node();
        virtual ~node();
        
        void                     add_child(ast::node* new_child);
        std::vector<ast::node*>* children(void) const;
        ast::node*               child_at_index(unsigned int i) const;
        int                      child_count() const;
        
        virtual std::string node_name(void);
        virtual std::string to_string(void);
        virtual std::string string_representation(int depth=0);
        void                print(void);
        
        virtual ast::node* parse(chime::parser* parser);
        
    protected:
        std::vector<ast::node*>* _children;
    };
    
    ast::node* construct(chime::parser* parser);
    ast::node* construct_expression(chime::parser* parser);
    ast::node* construct_valued_expression(chime::parser* parser);
}
#endif
