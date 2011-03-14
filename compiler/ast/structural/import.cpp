#include "import.h"
#include "../primary/entity_reference.h"

namespace ast
{
    import::import(chime::parser* parser)
    {
        // parse the import statement
        parser->next_token_value("import");
        
        this->expression(ast::construct_valued_expression(parser));
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
        this->add_child(n);
    }
}
