// chime: if_statement.h

#ifndef IF_STATEMENT_H
#define IF_STATEMENT_H

#include "compiler/ast/node.h"

namespace ast
{
    class IfStatement : public node
    {
    public:
        IfStatement(chime::parser& parser, ast::node* body);
        
        virtual std::string nodeName(void) const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        llvm::Value* codegen(chime::code_generator& generator);
        
        ast::NodeRef getCondition(void) const;
        void         setCondition(ast::NodeRef node);
        ast::NodeRef getBody(void) const;
        void         setBody(ast::NodeRef node);
        ast::NodeRef getElse(void) const;
        void         setElse(ast::NodeRef node);
    
    private:
        ast::NodeRef _condition;
        ast::NodeRef _body;
        ast::NodeRef _else;
    };
}
#endif // IF_STATEMENT_H
