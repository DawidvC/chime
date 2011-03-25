#include "if_statement.h"
#include "../../parser/parser.h"
#include <assert.h>

namespace ast
{
    if_statement::if_statement(chime::parser* parser, ast::node* body)
    {
        parser->next_token("if");
        
        this->condition(parser->parse_expression());
        
        if (body)
        {
            this->body(body);
        }
    }
    
    std::string if_statement::node_name(void)
    {
        return std::string("if statement");
    }
    
    ast::node* if_statement::condition(void) const
    {
        return this->child_at_index(0);
    }
    void if_statement::condition(ast::node* node)
    {
        assert(this->child_count() == 0);
        
        this->add_child(node);
    }
    ast::node* if_statement::body(void) const
    {
        return this->child_at_index(1);
    }
    void if_statement::body(ast::node* node)
    {
        assert(this->child_count() == 1);
        
        this->add_child(node);
    }
}
