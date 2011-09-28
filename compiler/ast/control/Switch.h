// chime: Switch.h

#ifndef SWITCH_H
#define SWITCH_H

#include "compiler/ast/node.h"
#include "Case.h"

namespace ast
{
    class Switch : public Node
    {
    public:
        static ast::Node* parse(chime::parser& parser);
        
    public:
        std::string nodeName(void) const;
        
        ast::NodeRef              getExpression() const;
        void                      setExpression(ast::NodeRef expression);
        std::vector<ast::CaseRef> getCases() const;
        ast::NodeRef              getElse() const;
        
    private:
        ast::NodeRef              _expression;
        std::vector<ast::CaseRef> _cases;
        ast::NodeRef              _else;
    };
}

#endif // SWITCH_H
