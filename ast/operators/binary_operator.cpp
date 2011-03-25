#include "binary_operator.h"
#include <assert.h>

namespace ast
{
    binary_operator::binary_operator()
    {
    }
    
    std::string binary_operator::node_name(void)
    {
        return std::string("binary operator");
    }
    std::string binary_operator::to_string(void)
    {
        std::string s;
        
        s += "operator: " + this->identifier();
        
        return s;
    }
    
    ast::node* binary_operator::right_operand(void) const
    {
        return this->child_at_index(1);
    }
    void binary_operator::right_operand(ast::node* op)
    {
        assert(this->child_count() == 1);
        assert(op != NULL);
        
        _children->push_back(op);
    }
    ast::node* binary_operator::left_operand(void) const
    {
        return this->child_at_index(0);
    }
    void binary_operator::left_operand(ast::node* op)
    {
        assert(this->child_count() == 0);
        assert(op != NULL);
        
        _children->push_back(op);
    }
}
