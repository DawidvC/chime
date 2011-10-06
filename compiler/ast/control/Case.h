// chime: Case.h

#ifndef CASE_H
#define CASE_H

#include "Conditional.h"

namespace ast
{
    class Case : public Conditional
    {
    public:
        static std::tr1::shared_ptr<ast::Case> parse(chime::parser& parser, Node* switchExpression);
        
    public:
        std::string nodeName(void) const;
        
        llvm::Value* codegen(chime::code_generator& generator);
    };
    
    typedef std::tr1::shared_ptr<ast::Case> CaseRef;
}

#endif // SWITCH_H
