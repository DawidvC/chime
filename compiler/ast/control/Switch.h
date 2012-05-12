// chime: Switch.h

#ifndef SWITCH_H
#define SWITCH_H

#include "compiler/ast/node.h"
#include "Case.h"

namespace chime
{
    class Switch : public Node
    {
    public:
        static Node* parse(chime::parser& parser);
        
    public:
        std::string nodeName() const;
        
        NodeRef              getExpression() const;
        void                 setExpression(NodeRef expression);
        std::vector<CaseRef> getCases() const;
        NodeRef              getElse() const;
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
        
    private:
        NodeRef              _expression;
        std::vector<CaseRef> _cases;
    };
}

#endif // SWITCH_H
