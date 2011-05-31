#include "method_definition.h"
#include "parser/parser.h"
#include "codegen/code_generator.h"

namespace ast
{
    method_definition::method_definition(chime::parser* parser) : code_container()
    {
        // method identifier
        parser->next_token_value("method");
        this->identifier(parser->next_token_value());
        
        this->parse_parameters(parser);
        this->parse_body(parser, false);
    }
    
    std::string method_definition::node_name(void)
    {
        return std::string("method definition");
    }
    std::string method_definition::to_string(void)
    {
        std::string s;
        
        s += "method: " + this->identifier();
        
        return s;
    }
    
    std::string method_definition::identifier() const
    {
        return _identifier;
    }
    void method_definition::identifier(std::string s)
    {
        _identifier = s;
    }
    
    llvm::Value* method_definition::codegen(chime::code_generator& generator)
    {
        llvm::Function*   method_function;
        llvm::Value*      target;
        llvm::BasicBlock* basic_block;
        
        method_function = llvm::Function::Create(generator.get_chime_function_type(), llvm::GlobalValue::ExternalLinkage, this->identifier(), generator.module());
        method_function->setCallingConv(llvm::CallingConv::C);
        
        basic_block = llvm::BasicBlock::Create(*generator.get_context(), "entry", method_function, 0);
        
        llvm::ReturnInst::Create(*generator.get_context(), generator.get_chime_literal_null(), basic_block);
        
        target = generator.current_method_target();
        
        generator.call_chime_object_set_function(target, this->identifier(), method_function, this->parameters()->size());
        
        return NULL;
    }
}
