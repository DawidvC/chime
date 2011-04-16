#include "binary_operator.h"
#include <assert.h>
#include "operations/code_generator.h"
#include "ast/primary/method_call.h"

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
        
        this->add_child(op);
    }
    ast::node* binary_operator::left_operand(void) const
    {
        return this->child_at_index(0);
    }
    void binary_operator::left_operand(ast::node* op)
    {
        assert(this->child_count() == 0);
        
        this->add_child(op);
    }
    
    llvm::Value* binary_operator::codegen(chime::code_generator& generator)
    {
        llvm::Value* l_value;
        llvm::Value* r_value;
        
        l_value = this->left_operand()->codegen(generator);
        
        assert(l_value != NULL);
        
        if (this->identifier().compare(".") == 0)
        {
            ast::method_call* call;
            
            call = dynamic_cast<ast::method_call*>(this->right_operand());
            assert(call);
            
            return generator.call_chime_object_invoke(l_value, call->identifier());
        }
        
        return NULL;
    }
}
