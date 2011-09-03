#include "Variable.h"
#include "compiler/ast/operators/IndexOperator.h"

namespace ast
{
    node* Variable::parse(chime::parser& parser)
    {
        std::string identifier;
        node*       node;
        
        assert(parser.look_ahead()->isIdentifier());
        
        identifier = parser.next_token_value();
        
        // Defer to the current scope to actually create the variable object.  This is
        // the trick to make scope-aware variables in the AST
        node = parser.getCurrentScope()->variableForIdentifier(identifier);
        
        if (parser.look_ahead(1)->equal_to("["))
        {
            node = ast::IndexOperator::parse(parser, node);
        }
        
        return node;
    }
    
    Variable::Variable(const std::string& identifier)
    {
        _identifier = identifier;
        _closed = false;
    }
    
    std::string Variable::getIdentifier() const
    {
        return _identifier;
    }
    
    bool Variable::getClosed() const
    {
        return _closed;
    }
    
    void Variable::setClosed(bool value)
    {
        _closed = value;
    }
}
