#include "import.h"

namespace ast
{
    import::import(chime::parser* parser)
    {
        chime::token* t;
        ast::node*    node;
        
        // parse the import statement
        parser->next_token_value("import");
        
        node = ast::construct_valued_expression(parser);
        
        t = parser->look_ahead();
        if (t->is_string())
        {
            //ast::node* n;
            
            //n = 
            printf("Found a string for the import\n");
        }
        else if (t->is_entity())
        {
            printf("Found an entity for the import\n");
        }
    }
    
    std::string import::node_name(void)
    {
        return std::string("import");
    }
    
    ast::node* import::expression() const
    {
        return this->child_at_index(0);
    }
    void import::expression(ast::node* n)
    {
        if (_children->size() != 0)
        {
            printf("importnode::expression failed\n");
            fflush(stdout);
            throw "madness";
        }
        
        _children->push_back(n);
    }
}
