#include "basic_operator.h"

#ifndef BINARY_OPERATOR_NODE
#define BINARY_OPERATOR_NODE

namespace ast
{
    class binary_operator : public basic_operator
    {
    public:
        binary_operator();
        
        virtual std::string node_name(void);
        virtual std::string to_string(void);
        
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
