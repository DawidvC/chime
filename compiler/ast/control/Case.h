// chime: Case.h

#ifndef CASE_H
#define CASE_H

#include "Conditional.h"

namespace chime
{
    class Case : public Conditional
    {
    public:
        static shared_ptr<Case> parse(parser& parser, Node* switchExpression);
        
    public:
        std::string nodeName(void) const;
    };
    
    typedef shared_ptr<Case> CaseRef;
}

#endif // SWITCH_H
