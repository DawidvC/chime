#include "import.h"
#include "parser/parser.h"
#include "ast/primary/type_reference.h"

namespace ast
{
    Import::Import(chime::parser& parser)
    {
        // parse the import statement
        parser.next_token_value("import");
        
        this->setImportand(parser.parse_type());
        parser.advance_past_ending_tokens();
    }
    
    Import::~Import()
    {
    }
    
    std::string Import::nodeName(void) const
    {
        return std::string("import");
    }
    
    ast::node* Import::getImportand() const
    {
        return this->childAtIndex(0);
    }
    void Import::setImportand(ast::node* n)
    {
        assert(this->childCount() == 0);
        assert(n != NULL);
        
        _children->push_back(n);
    }
}
