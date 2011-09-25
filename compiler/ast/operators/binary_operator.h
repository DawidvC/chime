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
        virtual std::string nodeName(void) const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        ast::node* getRightOperand() const;
        void       setRightOperand(ast::node* op);
        ast::node* right_operand(void) const;
        void       right_operand(ast::node* op);
        ast::node* getLeftOperand() const;
        void       setLeftOperand(ast::node* op);
        ast::node* left_operand(void) const;
        void       left_operand(ast::node* op);
        
        llvm::Value* codegen(chime::code_generator& generator);
    
    protected:
        llvm::Value* codegen_assignment(chime::code_generator& generator);
        
    private:
        int _precendence;
    };
    
    typedef ast::binary_operator BinaryOperator;
}

#endif
