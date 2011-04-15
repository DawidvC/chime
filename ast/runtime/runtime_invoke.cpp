#include "runtime_invoke.h"
#include <assert.h>
#include "operations/code_generator.h"

namespace ast
{
    runtime_invoke::runtime_invoke()
    {
        _property_name = "";
        _target        = NULL;
    }
    
    runtime_invoke::~runtime_invoke()
    {
        if (_target)
            delete _target;
    }
    
    std::string runtime_invoke::node_name(void)
    {
        return std::string("runtime invoke");
    }
    std::string runtime_invoke::to_string(void)
    {
        std::string s;
        
        s += "invoke: " + this->property_name() + " on " + _target->string_representation(0);
        
        return s;
    }
    
    ast::node* runtime_invoke::target() const
    {
        return _target;
    }
    
    void runtime_invoke::target(ast::node* node)
    {
        _target = node;
    }
    
    std::string runtime_invoke::property_name() const
    {
        return _property_name;
    }
    
    void runtime_invoke::property_name(std::string new_name)
    {
        _property_name = new_name;
    }
    
    llvm::Value* runtime_invoke::codegen(chime::code_generator& generator)
    {
        return NULL;
    }
}