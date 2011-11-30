#include "Parameter.h"

namespace chime
{
    ParameterRef Parameter::parse(parser& parser)
    {
        ParameterRef parameter;
        std::string  identifier;
        std::string  label;
        TypeRef      type;
        Token*       t;
        
        t = parser.look_ahead(2);
        if (t->equal_to(")") || t->equal_to(","))
        {
            // untyped identifier
            identifier = parser.nextTokenValue();
            
            parser.getCurrentScope()->defineIdentifier(identifier);
            
            return ParameterRef(new Parameter(identifier));
        }
        
        if (t->equal_to(":"))
        {
            // label:identifier
            label = parser.nextTokenValue();
            parser.nextTokenValue(":");
        }
        
        t = parser.look_ahead();
        if (t->isType())
        {
            type = TypeRef(new chime::Type(&parser));
        }
        
        identifier = parser.nextTokenValue();
        parser.getCurrentScope()->defineIdentifier(identifier);
        
        parameter = ParameterRef(new Parameter(identifier));
        parameter->_label = label;
        parameter->_type  = type;
        
        return parameter;
    }
    
    std::vector<ParameterRef> Parameter::parseList(parser& parser)
    {
        std::vector<ParameterRef> parameterSet;
        
        // this behavior is currently important, because Closure parsing depends on having optional opening
        // parens
        if (!parser.advanceTokenIfEqual("("))
            return parameterSet;
        
        if (parser.lookAhead(2)->equal_to(";"))
        {
            TypeRef type;
            
            // for now, we are abandoning the return value
            type = TypeRef(new Type(&parser));
            
            parser.nextTokenValue(";");
        }
        
        while (true)
        {
            Token* t;
            
            t = parser.look_ahead();
            
            if (t->empty() || t->equal_to(")"))
                break;
            
            parameterSet.push_back(Parameter::parse(parser));
            
            // take care of the commas
            parser.advanceTokenIfEqual(",");
        }
        
        parser.nextTokenValue(")");
        
        return parameterSet;
    }
    
    Parameter::Parameter(const std::string& identifier) :
        LocalVariable(identifier)
    {
        this->setDefined(true);
    }
    
    std::string Parameter::nodeName() const
    {
        return "Parameter";
    }
    
    std::string Parameter::label() const
    {
        return _label;
    }
    
    void Parameter::setLabel(const std::string& s)
    {
        _label = s;
    }
    
    TypeRef Parameter::type() const
    {
        return _type;
    }
}
