// chime: While.h

#ifndef WHILE_H
#define WHILE_H

#include "compiler/ast/node.h"

namespace ast
{
    class While : public Node
    {
    public:
        static ast::While* parse(chime::parser& parser);
        
    public:
        NodeRef getCondition() const;
        void    setCondition(NodeRef node);
        NodeRef getBody() const;
        void    setBody(NodeRef node);
        
        llvm::Value* codegen(chime::code_generator& generator);
        
    private:
        ast::NodeRef _condition;
        ast::NodeRef _body;
    };
    
    typedef shared_ptr<While> WhileRef;
}
#endif // WHILE_H
