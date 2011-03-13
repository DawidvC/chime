#include "binary_operator.h"

namespace ast
{
    binary_operator::binary_operator(chime::parser* parser, ast::node* left_operand)
    {
        this->identifier(parser->next_token_value());
        
        this->left_operand(left_operand);
        this->right_operand(construct_valued_expression(parser));
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
        if (_children->size() != 1)
        {
            printf("right_operator failed\n");
            fflush(stdout);
            throw "madness";
        }
        
        _children->push_back(op);
    }
    ast::node* binary_operator::left_operand(void) const
    {
        return this->child_at_index(0);
    }
    void binary_operator::left_operand(ast::node* op)
    {
        if (_children->size() != 0)
        {
            printf("left_operator failed\n");
            fflush(stdout);
            throw "madness";
        }
        
        _children->push_back(op);
    }
}
