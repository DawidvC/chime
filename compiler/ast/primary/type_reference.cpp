#include "type_reference.h"
#include "compiler/ast/common/parameter_set.h"

namespace ast
{
    type_reference::type_reference(chime::parser* parser)
    {
        std::string str;
        
        assert(parser->look_ahead()->is_type());
        
        str += parser->next_token_value();
        
        while (parser->look_ahead()->equal_to(".") && parser->look_ahead(2)->is_type())
        {
            parser->next_token_value(".");
            
            str += ".";
            str +=parser->next_token_value();
        }
        
        this->identifier(str);
        
        if (this->identifier() == "Function")
        {
            // ( parameters )
            if (parser->look_ahead()->equal_to("("))
                _parameters = ParameterSetRef(new ParameterSet(*parser));
        }
    }
    
    std::string type_reference::nodeName(void) const
    {
        return std::string("type reference");
    }
    std::string type_reference::toString(void) const
    {
        std::string s;
        
        s += "type: " + this->identifier();
        
        return s;
    }
    
    std::string type_reference::identifier() const
    {
        return _identifier;
    }
    void type_reference::identifier(std::string s)
    {
        _identifier = s;
    }
    
    llvm::Value* type_reference::codegen(chime::code_generator& generator)
    {
        llvm::Value* class_name_ptr;
        
        class_name_ptr = generator.make_constant_string(this->identifier());
        
        return generator.getRuntime()->callChimeRuntimeGetClass(class_name_ptr);
    }
}