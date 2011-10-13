#ifndef INDEX_OPERATOR_H
#define INDEX_OPERATOR_H

#include "basic_operator.h"

namespace ast
{
    class IndexOperator : public basic_operator
    {
    public:
        static ast::node* parse(chime::parser& parser, ast::node* operand);
        
    public:
        std::string nodeName(void) const;
        std::string stringRepresentation(int depth=0) const;
        
        ast::NodeRef getOperand(void) const;
        void         setOperand(ast::NodeRef node);
        ast::NodeRef getArgument(void) const;
        void         setArgument(ast::NodeRef node);
        
        llvm::Value* codegen(chime::code_generator& generator);
        
    private:
        ast::NodeRef _operand;
        ast::NodeRef _argument;
    };
}

#endif
