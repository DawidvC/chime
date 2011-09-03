#include "Structural.h"
#include "Implementation.h"
#include "method_definition.h"
#include "PropertyDefinition.h"
#include "Attribute.h"
#include "import.h"
#include "compiler/parser/parser.h"

namespace ast
{
    ast::node* Structural::parse(chime::parser& parser)
    {
        ast::node*    node;
        chime::token* t;
        
        node = NULL;
        t    = parser.look_ahead();
        
        if (t->empty())
            return NULL;
        
        if (t->equal_to("import"))
        {
            node = new ast::Import(parser);
        }
        else if (t->equal_to("implementation"))
        {
            node = new ast::Implementation(parser);
        }
        else if (t->equal_to("method"))
        {
            node = ast::method_definition::parse(parser);
        }
        else if (t->equal_to("property"))
        {
            node = new ast::PropertyDefinition(parser);
        }
        else if (t->equal_to("attribute"))
        {
            node = new ast::Attribute(parser);
        }
        else
        {
            node = parser.parse_without_structural();
        }
        
        if (!node)
        {
            parser.addError("Unable to parse a structural or non-structural element");
        }
        
        return node;
    }
}
