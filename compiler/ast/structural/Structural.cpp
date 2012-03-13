#include "Structural.h"
#include "implementation.h"
#include "IndexerDefinition.h"
#include "method_definition.h"
#include "OperatorDefinition.h"
#include "PropertyDefinition.h"
#include "Attribute.h"
#include "import.h"
#include "Include.h"
#include "Trait.h"
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
            node = ast::Implementation::parse(parser);
        }
        else if (t->equal_to("trait"))
        {
            node = chime::Trait::parse(parser);
        }
        else if (parser.advanceTokenIfEqual("class"))
        {
            node = ast::method_definition::parse(parser);
            
            static_cast<chime::MethodDefinition*>(node)->setInstance(false);
        }
        else if (t->equal_to("method"))
        {
            node = ast::method_definition::parse(parser);
            
            static_cast<chime::MethodDefinition*>(node)->setInstance(true);
        }
        else if (t->equal_to("property"))
        {
            node = ast::PropertyDefinition::parse(parser);
        }
        else if (t->equal_to("indexer"))
        {
            node = chime::IndexerDefinition::parse(parser);
        }
        else if (t->equal_to("operator"))
        {
            node = chime::OperatorDefinition::parse(parser);
        }
        else if (t->equal_to("attribute"))
        {
            node = new ast::Attribute(parser);
        }
        else if (t->equal_to("include"))
        {
            node = chime::Include::parse(parser);
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
