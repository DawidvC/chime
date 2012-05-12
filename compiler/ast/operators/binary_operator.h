#ifndef BINARY_OPERATOR_NODE
#define BINARY_OPERATOR_NODE

#include "basic_operator.h"

namespace chime
{
    class binary_operator : public basic_operator
    {
    public:
        static Node* parse(chime::parser& parser, int precedence, Node* leftOperand);
        static Node* parseRightOperand(chime::parser& parser);
        
    public:
        virtual std::string nodeName(void) const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        Node* getRightOperand() const;
        void  setRightOperand(Node* op);
        Node* right_operand(void) const;
        void  right_operand(Node* op);
        Node* getLeftOperand() const;
        void  setLeftOperand(Node* op);
        Node* left_operand(void) const;
        void  left_operand(Node* op);
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
        
    private:
        int _precendence;
    };
    
    typedef chime::binary_operator BinaryOperator;
}

#endif
