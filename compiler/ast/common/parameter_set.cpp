// chime: parameter_set.cpp

#include "parameter_set.h"

namespace chime
{
    ParameterSetRef ParameterSet::parse(parser& parser)
    {
        ParameterSetRef parameterSet;
        
        parameterSet = ParameterSetRef(new ParameterSet());
        
        // this behavior is currently important, because Closure parsing depends on having optional opening
        // parens
        if (!parser.advanceTokenIfEqual("("))
            return parameterSet;
        
        if (parser.lookAhead(2)->equal_to(";"))
        {
            parameterSet->setReturnType(TypeRef(new type_reference(&parser)));
            parser.nextTokenValue(";");
        }
        
        while (true)
        {
            chime::token* t;
            
            t = parser.lookAhead();
            
            if (t->empty() || t->equal_to(")"))
                break;
            
            parameterSet->addChild(new ast::method_parameter(&parser));
            
            // take care of the commas
            parser.advanceTokenIfEqual(",");
        }
        
        parser.next_token_value(")");
        
        return parameterSet;
    }
    
    std::string ParameterSet::nodeName() const
    {
        return "parameter set";
    }
    
    unsigned int ParameterSet::length() const
    {
        return this->childCount();
    }
    
    ast::method_parameter* ParameterSet::parameterAtIndex(unsigned int i) const
    {
        return static_cast<ast::method_parameter*>(this->childAtIndex(i));
    }
    
    TypeRef ParameterSet::getReturnType() const
    {
        return _returnType;
    }
    
    void ParameterSet::setReturnType(TypeRef type)
    {
        _returnType = type;
    }
}
