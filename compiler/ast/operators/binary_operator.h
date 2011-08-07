#ifndef BINARY_OPERATOR_NODE
#define BINARY_OPERATOR_NODE

#include "basic_operator.h"

namespace ast
{
    class binary_operator : public basic_operator
    {
    public:
        static ast::node* parse(chime::parser& parser, int precedence, ast::node* leftOperand);
        static ast::node* parseRightOperand(chime::parser& parser);
        
    public:
        binary_operator();
        
        virtual std::string node_name(void) const;
        virtual std::string to_string(void) const;
        
        ast::node* right_operand(void) const;
        void       right_operand(ast::node* op);
        ast::node* left_operand(void) const;
        void       left_operand(ast::node* op);
        
        llvm::Value* codegen(chime::code_generator& generator);
        
    protected:
        int _precendence;
    };
}

#endif
