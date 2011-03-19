#include "import.h"
#include "../primary/type_reference.h"

namespace ast
{
    import::import(chime::parser* parser)
    {
        // parse the import statement
        parser->next_token_value("import");
        
        if (parser->look_ahead()->is_string())
        {
            fprintf(stderr, "not yet able to handle string imports\n");
        }
        else
        {
            this->importand(new ast::type_reference(parser));
        }
    }
    
    std::string import::node_name(void)
    {
        return std::string("import");
    }
    
    ast::type_reference* import::importand() const
    {
        return (ast::type_reference*)this->child_at_index(0);
    }
    void import::importand(ast::type_reference* n)
    {
        this->add_child(n);
    }
}
