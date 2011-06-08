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
        llvm::Function*   methodFunction;
        llvm::Value*      target;
        llvm::BasicBlock* basicBlock;
        
        methodFunction = llvm::Function::Create(generator.getRuntime()->getChimeFunctionType(), llvm::GlobalValue::ExternalLinkage, this->identifier(), generator.module());
        methodFunction->setCallingConv(llvm::CallingConv::C);
        
        basicBlock = llvm::BasicBlock::Create(generator.get_context(), "entry", methodFunction, 0);
        generator.builder()->SetInsertPoint(basicBlock);
        
        // body goes here
        
        generator.builder()->CreateRet(generator.get_chime_literal_null());
        
        target = generator.current_method_target();
        
        generator.call_chime_object_set_function(target, this->identifier(), methodFunction, this->parameters()->size());
        
        // finally, verify the functiont to be sure we didn't mess anything up
        llvm::verifyFunction(*methodFunction);
        
        return NULL;
    }
}
