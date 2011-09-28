// chime: Case.h

#ifndef CASE_H
#define CASE_H

#include "compiler/ast/node.h"

namespace ast
{
    class Case : public Node
    {
    public:
        static std::tr1::shared_ptr<ast::Case> parse(chime::parser& parser);
        
    public:
        std::string nodeName(void) const;
        
        ast::NodeRef getCondition() const;
        void         setCondition(ast::NodeRef expression);
        ast::NodeRef getBody(void) const;
        void         setBody(ast::NodeRef node);
        
    private:
        ast::NodeRef _condition;
        ast::NodeRef _body;
    };
    
    typedef std::tr1::shared_ptr<ast::Case> CaseRef;
}

#endif // SWITCH_H
