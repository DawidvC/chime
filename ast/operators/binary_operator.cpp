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
        llvm::Value*              l_value;
        llvm::Value*              r_value;
        llvm::LoadInst*           object_load;
        std::vector<llvm::Value*> arguments;
        
        l_value = this->left_operand()->codegen(generator);
        assert(l_value != NULL);
        
        if (this->identifier().compare(".") == 0)
        {
            ast::method_call* call;
            llvm::Value*      argument_value;
            
            call = dynamic_cast<ast::method_call*>(this->right_operand());
            assert(call);
            
            // arguments need to be filled in here!
            std::vector<ast::node*>::iterator i;
            for (i=call->children()->begin(); i < call->children()->end(); i++)
            {
                argument_value = (*i)->codegen(generator);
                assert(argument_value);
                
                object_load = generator.builder()->CreateLoad(argument_value, "loaded argument");
                
                arguments.push_back(object_load);
            }
            
            return generator.call_chime_object_invoke(l_value, call->identifier(), arguments);
        }
        
        r_value = this->right_operand()->codegen(generator);
        assert(r_value != NULL);
        
        object_load = generator.builder()->CreateLoad(r_value, "loaded r_value");
        
        if (this->identifier().compare("=") == 0)
        {
            generator.builder()->CreateStore(object_load, l_value, false);
            
            return l_value;
        }
        
        arguments.push_back(object_load);
        
        return generator.call_chime_object_invoke(l_value, this->identifier(), arguments);
    }
}
