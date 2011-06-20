// chime: parameter_set.h

#ifndef PARAMETER_SET_H
#define PARAMETER_SET_H

#include "ast/node.h"

namespace ast
{
    class ParameterSet : public node
    {
    public:
        ParameterSet(chime::parser& parser);
        virtual ~ParameterSet();
        
        virtual std::string  nodeName(void) const;
    };
    
    typedef shared_ptr<ast::ParameterSet> ParameterSetRef;
}
#endif // PARAMETER_SET_H
