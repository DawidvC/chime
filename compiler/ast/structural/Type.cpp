#include "Type.h"
#include "compiler/ast/variable/Parameter.h"

namespace chime
{
    Type::Type(chime::parser* parser)
    {
        std::string str;
        
        assert(parser->look_ahead()->isType());
        
        str += parser->next_token_value();
        
        while (parser->look_ahead()->equal_to(".") && parser->look_ahead(2)->isType())
        {
            parser->next_token_value(".");
            
            str += ".";
            str += parser->next_token_value();
        }
        
        this->setIdentifier(str);
        
        if (this->identifier() == "Function")
        {
            // ( parameters )
            if (parser->look_ahead()->equal_to("("))
                _parameters = chime::Parameter::parseList(*parser);
        }
    }
    
    std::string Type::nodeName() const
    {
        return "Type";
    }
    std::string Type::stringRepresentation(int depth) const
    {
        std::string str;
        
        str.append(depth*2, ' ');
        str.append("type: ");
        str.append(this->identifier());
        
        return str;
    }
    
    std::string Type::identifier() const
    {
        return _identifier;
    }
    void Type::setIdentifier(const std::string& s)
    {
        _identifier = s;
    }
    
    llvm::Value* Type::codegen(chime::code_generator& generator)
    {
        llvm::Value* class_name_ptr;
        
        class_name_ptr = generator.make_constant_string(this->identifier());
        
        return generator.getRuntime()->callChimeRuntimeGetClass(class_name_ptr);
    }
}