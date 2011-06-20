// chime: method_definition.cpp

#include "method_definition.h"
#include "parser/parser.h"
#include "codegen/code_generator.h"

namespace ast
{
    method_definition::method_definition(chime::parser* parser)
    {
        // method identifier
        parser->next_token_value("method");
        this->identifier(parser->next_token_value());
        
        _parameters = ParameterSetRef(new ParameterSet(*parser));
        
        _bodyBlock = CodeBlock::nextBlock(*parser);
    }
    
    std::string method_definition::node_name(void) const
    {
        return std::string("method definition");
    }
    std::string method_definition::to_string(void) const
    {
        std::string s;
        
        s += "method: " + this->identifier();
        
        return s;
    }
    
    std::string method_definition::identifier(void) const
    {
        return _identifier;
    }
    void method_definition::identifier(std::string s)
    {
        _identifier = s;
    }
    std::string method_definition::getName(void) const
    {
        return this->identifier();
    }
    
    NodeRef method_definition::getBody(void) const
    {
        return _bodyBlock;
    }
    ParameterSetRef method_definition::getParameters() const
    {
        return _parameters;
    }
    
    llvm::Value* method_definition::codegen(chime::code_generator& generator)
    {
        llvm::Function*   methodFunction;
        std::string       functionName;
        llvm::Value*      functionNameCStringPtr;
        llvm::BasicBlock* basicBlock;
        llvm::BasicBlock* currentBlock;
        llvm::Value*      classObjectPtr;
        
        currentBlock = generator.builder()->GetInsertBlock();
        
        functionName           = generator.getImplementationScope()->getName() + "." + this->getName();
        functionNameCStringPtr = generator.make_constant_string(this->getName());
        
        methodFunction = generator.createFunction(generator.getRuntime()->getChimeFunctionType(), functionName);
        
        basicBlock = llvm::BasicBlock::Create(generator.getContext(), "entry", methodFunction, 0);
        generator.builder()->SetInsertPoint(basicBlock);
        
        // create the actual method body
        this->getBody()->codegen(generator);
        
        generator.builder()->CreateRet(generator.get_chime_literal_null());
        
        // before we continue, verify the function
        llvm::verifyFunction(*methodFunction);
        
        // restore the builder's position
        generator.builder()->SetInsertPoint(currentBlock);
        
        // get the meta-class
        classObjectPtr = generator.getImplementationScope()->getTarget();
        
        // finally, install the method in the class
        generator.getRuntime()->callChimeObjectSetFunction(classObjectPtr, functionNameCStringPtr, methodFunction, this->getParameters()->child_count());
        
        return NULL;
    }
}
