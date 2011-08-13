// chime: parameter_set.h

#ifndef PARAMETER_SET_H
#define PARAMETER_SET_H

#include "compiler/ast/node.h"
#include "compiler/ast/common/method_parameter.h"

namespace ast
{
    class ParameterSet : public node
    {
    public:
        ParameterSet(chime::parser& parser);
        virtual ~ParameterSet();
        
        virtual std::string  nodeName(void) const;
        
        unsigned int           length() const;
        ast::method_parameter* parameterAtIndex(unsigned int i) const;
    };
    
    typedef std::tr1::shared_ptr<ast::ParameterSet> ParameterSetRef;
}
#endif // PARAMETER_SET_H
