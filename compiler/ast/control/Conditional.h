// chime: Conditional.h

#ifndef CONDITIONAL_H
#define CONDITIONAL_H

#include "compiler/ast/node.h"

namespace chime
{
    class Conditional : public Node
    {
    public:
        static Node* parse(parser& parser, Node* body=NULL);
        static Node* parseTailing(parser& parser, Node* body);
        
    public:
        virtual std::string stringRepresentation(int depth=0) const;
        
        NodeRef getCondition() const;
        void    setCondition(NodeRef node);
        NodeRef getBody() const;
        void    setBody(NodeRef node);
        NodeRef getElse() const;
        void    setElse(NodeRef node);
        
        llvm::Value* codegenConditional(code_generator& generator, llvm::BasicBlock* endBlock, bool negate=false);
        
    private:
        NodeRef _condition;
        NodeRef _body;
        NodeRef _else;
    };
}
#endif // CONDITIONAL_H
