// chime: Conditional.h

#ifndef CONDITIONAL_H
#define CONDITIONAL_H

#include "compiler/ast/node.h"

namespace ast
{
    class Conditional : public Node
    {
    public:
        NodeRef getCondition() const;
        void    setCondition(NodeRef node);
        NodeRef getBody() const;
        void    setBody(NodeRef node);
        NodeRef getElse() const;
        void    setElse(NodeRef node);
        
        llvm::Value* codegenConditional(chime::code_generator& generator, llvm::BasicBlock* elseBlock, llvm::BasicBlock* endBlock);
        
    private:
        ast::NodeRef _condition;
        ast::NodeRef _body;
        ast::NodeRef _else;
    };
}
#endif // CONDITIONAL_H
