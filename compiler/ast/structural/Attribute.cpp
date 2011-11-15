#include "Attribute.h"

namespace ast
{
    Attribute::Attribute(chime::parser& parser)
    {
        chime::Variable* v;
        
        parser.next_token_value("attribute");
        
        if (!parser.look_ahead()->isIdentifier())
        {
            parser.addError("We need an identifier after an attribute...");
            return;
        }
        
        _identifier = parser.next_token_value();
        
        // slight hack to give the current scope (an Implemenation), a chance
        // to define the variable.  We don't actually use the variable here.
        
        assert(parser.getCurrentScope()->nodeName() == "implementation");
        
        v = parser.getCurrentScope()->variableForIdentifier(_identifier);
        
        assert(v->nodeName() == "Instance Variable");
    }
        
    std::string Attribute::nodeName() const
    {
        return std::string("attribute");
    }
    
    std::string Attribute::stringRepresentation(int depth) const
    {
        std::string str;
        
        str.append(depth*2, ' ');
        str.append(this->nodeName());
        str.append(": ");
        str.append(this->getIdentifier());
        str.append("\n");
        
        return str;
    }
    
    std::string Attribute::getIdentifier() const
    {
        return _identifier;
    }
    
    llvm::Value* Attribute::codegen(chime::code_generator& generator)
    {
        // Here's something interesting.  We don't need to create code here, 
        // since the parser now knows that this symbol is an ivar
        
        return NULL;
    }
}
