// chime: FunctionDefinition.cpp

#include "FunctionDefinition.h"
#include "compiler/codegen/code_generator.h"

namespace ast
{
    FunctionDefinition::FunctionDefinition()
    {
    }
    
    std::string FunctionDefinition::getIdentifier(void) const
    {
        return _identifier;
    }
    void FunctionDefinition::setIdentifier(const std::string& s)
    {
        _identifier = s;
    }
    ParameterSetRef FunctionDefinition::getParameters() const
    {
        return _parameters;
    }
    void FunctionDefinition::setParameters(ParameterSetRef params)
    {
        _parameters = params;
    }
    
    llvm::Function* FunctionDefinition::createFunction(chime::code_generator& generator, const std::string& name, unsigned int arity)
    {
        llvm::Function*                function;
        llvm::FunctionType*            functionType;
        std::vector<const llvm::Type*> functionArgs;
        unsigned int                   i;
        
        // setup the function type
        functionArgs.push_back(generator.getRuntime()->getChimeObjectPtrType()); // first argument is always self
        
        // add a new argument for each parameter
        for (i = 0; i < arity; ++i)
        {
            functionArgs.push_back(generator.getRuntime()->getChimeObjectPtrType()); // this is arg1
        }
        
        functionType = llvm::FunctionType::get(generator.getRuntime()->getChimeObjectPtrType(), functionArgs, false);
        
        function = generator.createFunction(functionType, name);
        
        return function;
    }
    
    void FunctionDefinition::codegenArguments(chime::code_generator& generator, llvm::Function* function)
    {
        llvm::Function::arg_iterator args;
        llvm::AllocaInst*            alloca;
        unsigned int                 i;
        
        // first, deal with self
        args = function->arg_begin();
        
        alloca = generator.insertChimeObjectAlloca();
        generator.builder()->CreateStore(args, alloca, false);
        
        generator.getCurrentScope()->setValueForIdentifier("_self", alloca);
        
        ++args; // advance past self argument
        
        assert(this->getParameters());
        
        // Careful here.  Properties define two methods, one that has params and one that does not.  The
        // safest thing to do here is to iterate over the function arguments, and fill in as we go
        
        for (i = 0; args != function->arg_end(); ++args, ++i)
        {
            ast::method_parameter* param;
            llvm::AllocaInst*      alloca;
            
            assert(args != function->arg_end());
            
            param = this->getParameters()->parameterAtIndex(i);
            
            alloca = generator.insertChimeObjectAlloca();
            generator.builder()->CreateStore(args, alloca, false);
            generator.getCurrentScope()->setValueForIdentifier(param->identifier(), alloca);
        }
    }
    
    llvm::Function* FunctionDefinition::codegenFunction(chime::code_generator& generator, const std::string& name, NodeRef body, unsigned int arity)
    {
        llvm::Function*   function;
        llvm::BasicBlock* basicBlock;
        llvm::BasicBlock* currentBlock;
        
        function = this->createFunction(generator, name, arity);
        
        // capture the current insertion point
        currentBlock = generator.builder()->GetInsertBlock();
        
        generator.pushScope(this);
        
        basicBlock = llvm::BasicBlock::Create(generator.getContext(), "entry", function, 0);
        generator.builder()->SetInsertPoint(basicBlock);
        
        // arguments
        this->codegenArguments(generator, function);
        
        // now body
        body->codegen(generator);
        
        // only do this if we haven't yet terminated the block
        if (!generator.builder()->GetInsertBlock()->getTerminator())
        {
            generator.builder()->CreateRet(generator.getRuntime()->getChimeLiteralNull());
        }
        
        llvm::verifyFunction(*function);
        
        // restore the builder's position
        generator.popScope();
        generator.builder()->SetInsertPoint(currentBlock);
        
        return function;
    }
    
    llvm::Value* FunctionDefinition::createMethod(chime::code_generator& generator, const std::string& name, NodeRef body, unsigned int arity)
    {
        llvm::Function* methodFunction;
        std::string     functionName;
        llvm::Value*    functionNameCStringPtr;
        llvm::Value*    classObjectPtr;
        
        functionName           = generator.getImplementationScope()->getName() + "." + name;
        functionNameCStringPtr = generator.getConstantString(name);
        
        methodFunction = this->codegenFunction(generator, name, body, arity);
        
        // get the class object
        classObjectPtr = generator.getImplementationScope()->getTarget();
        
        // finally, install the method in the class
        generator.getRuntime()->callChimeRuntimeSetInstanceMethod(classObjectPtr, functionNameCStringPtr, methodFunction);
        
        return NULL;
    }
}
