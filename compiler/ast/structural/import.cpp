#include "import.h"
#include <assert.h>
#include "../primary/type_reference.h"
#include "../../parser/parser.h"

namespace ast
{
    import::import(chime::parser* parser)
    {
        // parse the import statement
        parser->next_token_value("import");
        
        this->importand(parser->parse_expression());
    }
    
    import::~import()
    {
    }
    
    std::string import::node_name(void)
    {
        return std::string("import");
    }
    
    ast::node* import::importand() const
    {
        return this->child_at_index(0);
    }
    void import::importand(ast::node* n)
    {
        assert(this->child_count() == 0);
        assert(n != NULL);
        
        _children->push_back(n);
    }
}
