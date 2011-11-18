// chime: Parameter.h

#ifndef PARAMETER_H
#define PARAMETER_H

#include "compiler/ast/structural/Type.h"
#include "LocalVariable.h"

namespace chime
{
    class Parameter : public ast::LocalVariable
    {
    public:
        static shared_ptr<Parameter>                parse(parser& parser);
        static std::vector< shared_ptr<Parameter> > parseList(parser& parser);
        
    public:
        Parameter(const std::string& identifier);
        
        std::string nodeName() const;
        
        std::string label() const;
        void        setLabel(const std::string& s);
        
        TypeRef     type() const;
        
        bool        isParameter() const;
        
    private:
        std::string _label;
        TypeRef     _type; // this should really be part of a variable
    };
    
    typedef shared_ptr<Parameter> ParameterRef;
}

#endif // PARAMETER_H
