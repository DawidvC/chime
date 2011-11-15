// chime: FunctionDefinition.cpp

#include "FunctionDefinition.h"
#include "compiler/codegen/code_generator.h"

namespace ast
{
    FunctionDefinition::FunctionDefinition()
    {
        _isInstance = true;
    }
    
    std::string FunctionDefinition::getIdentifier(void) const
    {
        return _identifier;
    }
    void FunctionDefinition::setIdentifier(const std::string& s)
    {
        _identifier = s;
    }
    chime::ParameterSetRef FunctionDefinition::getParameters() const
    {
        return _parameters;
    }
    void FunctionDefinition::setParameters(chime::ParameterSetRef params)
    {
        _parameters = params;
    }
    
    bool FunctionDefinition::isInstance() const
    {
        return _isInstance;
    }
    
    void FunctionDefinition::setInstance(bool value)
    {
        _isInstance = value;
    }
    
    chime::SelfLiteral* FunctionDefinition::createSelf()
    {
        return new chime::SelfLiteral();
    }
    
    bool FunctionDefinition::isFunction() const
    {
        return true;
    }
    
    void FunctionDefinition::defineParametersAsLocalVariables(chime::parser& parser)
    {
        // we now need to create variables (which should be strictly local) for
        // all the parameters
        for (unsigned int i = 0; i < this->getParameters()->length(); ++i)
        {
            ast::method_parameter* parameter;
            chime::Variable*       variable;
            
            parameter = this->getParameters()->parameterAtIndex(i);
            
            variable = this->variableForIdentifier(parameter->identifier());
            if (variable->nodeName() != "Local Variable")
            {
                parser.addError("Method argument shadows a variable in scope");
            }
        }
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
        
        generator.getCurrentScope()->setSelfObjectPtr(alloca);
        
        ++args; // advance past self argument
        
        if (!this->getParameters())
            return;
        
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
            
            // parameters are all +0 objects.  If we tracked them differently from local variables,
            // we could be a lot more efficient here.  For now, just retain them.
            
            generator.getRuntime()->callChimeObjectRetain(alloca);
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
        
        // pop our scope
        generator.popScope();
        
        // only do this if we haven't yet terminated the block
        if (!generator.builder()->GetInsertBlock()->getTerminator())
        {
            generator.builder()->CreateRet(generator.getRuntime()->getChimeLiteralNull());
        }
        
        llvm::verifyFunction(*function);
        
        // restore the builder's position
        generator.builder()->SetInsertPoint(currentBlock);
        
        return function;
    }
    
    llvm::Value* FunctionDefinition::createMethod(chime::code_generator& generator, const std::string& name, NodeRef body, unsigned int arity)
    {
        llvm::Function* methodFunction;
        std::string     functionName;
        llvm::Value*    functionNameCStringPtr;
        llvm::Value*    classObjectPtr;
        
        assert(generator.currentImplementation());
        
        functionName           = generator.currentImplementation()->getIdentifier() + "." + name;
        functionNameCStringPtr = generator.getConstantString(name);
        
        fprintf(stderr, "name: %s\n", functionName.c_str());
        
        // we need to scope the method name to include the class, so we don't overlap in the
        // tranlation unit
        
        methodFunction = this->codegenFunction(generator, functionName, body, arity);
        
        assert(generator.currentImplementation());
        
        // get the class object
        classObjectPtr = generator.currentImplementation()->getClassObjectPtr();
        assert(classObjectPtr);
        
        // finally, install the method in the class
        if (this->isInstance())
            generator.getRuntime()->callChimeRuntimeSetInstanceMethod(classObjectPtr, functionNameCStringPtr, methodFunction);
        else
            generator.getRuntime()->callChimeRuntimeSetClassMethod(classObjectPtr, functionNameCStringPtr, methodFunction);
            
        return NULL;
    }
}
