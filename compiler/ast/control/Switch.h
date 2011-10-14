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
        std::string nodeName() const;
        
        ast::NodeRef                getExpression() const;
        void                        setExpression(ast::NodeRef expression);
        std::vector<chime::CaseRef> getCases() const;
        ast::NodeRef                getElse() const;
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
        
    private:
        ast::NodeRef                _expression;
        std::vector<chime::CaseRef> _cases;
    };
}

#endif // SWITCH_H
