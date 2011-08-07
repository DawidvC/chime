// chime: method_definition.cpp

#include "method_definition.h"
#include "compiler/parser/parser.h"
#include "compiler/codegen/code_generator.h"

namespace ast
{
    method_definition::method_definition(chime::parser* parser)
    {
        // "method identifier"
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
        llvm::BasicBlock* returnBlock;
        llvm::BasicBlock* currentBlock;
        llvm::Value*      classObjectPtr;
        
        currentBlock = generator.builder()->GetInsertBlock();
        
        functionName           = generator.getImplementationScope()->getName() + "." + this->getName();
        functionNameCStringPtr = generator.make_constant_string(this->getName());
        
        methodFunction = generator.createFunction(generator.getRuntime()->getChimeFunctionType(), functionName);
        
        // create the return code for the method
        generator.setMethodScope(chime::MethodScopeRef(new chime::MethodScope()));
        
        returnBlock = llvm::BasicBlock::Create(generator.getContext(), "return");
        generator.getMethodScope()->setReturnBlock(returnBlock);
        
        generator.builder()->SetInsertPoint(returnBlock);
        generator.builder()->CreateRet(generator.getRuntime()->getChimeLiteralNull());
        
        // setup the function entry
        basicBlock = llvm::BasicBlock::Create(generator.getContext(), "entry", methodFunction, 0);
        generator.builder()->SetInsertPoint(basicBlock);
        
        // create the actual method body
        this->getBody()->codegen(generator);
        
        // now, add the return block on to the end
        methodFunction->getBasicBlockList().push_back(generator.getMethodScope()->getReturnBlock());
        
        // finally create a branch to the return block
        generator.builder()->CreateBr(generator.getMethodScope()->getReturnBlock());
        
        //generator.builder()->CreateRet(generator.getRuntime()->getChimeLiteralNull());
        
        // before we continue, verify the function
        llvm::verifyFunction(*methodFunction);
        
        // restore the builder's position
        generator.builder()->SetInsertPoint(currentBlock);
        
        // get the class object
        classObjectPtr = generator.getImplementationScope()->getTarget();
        
        // finally, install the method in the class
        generator.getRuntime()->callChimeObjectSetFunction(classObjectPtr, functionNameCStringPtr, methodFunction, this->getParameters()->child_count());
        
        return NULL;
    }
}
