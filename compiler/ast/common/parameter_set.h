// chime: parameter_set.h

#ifndef PARAMETER_SET_H
#define PARAMETER_SET_H

#include "compiler/ast/node.h"
#include "compiler/ast/common/method_parameter.h"

namespace chime
{
    class ParameterSet : public Node
    {
    public:
        static shared_ptr<ParameterSet> parse(parser& parser);
    
    public:
        virtual std::string    nodeName() const;
        
        unsigned int           length() const;
        ast::method_parameter* parameterAtIndex(unsigned int i) const;
        
        TypeRef                getReturnType() const;
        void                   setReturnType(TypeRef type);
        
    private:
        TypeRef _returnType;
    };
    
    typedef shared_ptr<ParameterSet> ParameterSetRef;
}
#endif // PARAMETER_SET_H
