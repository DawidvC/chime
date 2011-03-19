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
    
    import::~import()
    {
        delete _importand;
    }
    
    std::string import::node_name(void)
    {
        return std::string("import");
    }
    std::string import::to_string(void)
    {
        std::string s;
        
        s += "import: " + this->importand()->identifier();
        
        return s;
    }
    
    ast::type_reference* import::importand() const
    {
        return _importand;
    }
    void import::importand(ast::type_reference* n)
    {
        _importand = n;
    }
}
