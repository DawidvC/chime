#ifndef INDEX_OPERATOR_H
#define INDEX_OPERATOR_H

#include "basic_operator.h"

namespace chime
{
    class IndexOperator : public basic_operator
    {
    public:
        static Node* parse(chime::parser& parser, Node* operand, bool allowAssignment=false);
        
    public:
        virtual std::string nodeName(void) const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        NodeRef getOperand(void) const;
        void    setOperand(NodeRef node);
        NodeRef getArgument(void) const;
        void    setArgument(NodeRef node);
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
        
    private:
        NodeRef _operand;
        NodeRef _argument;
    };
}

#endif
