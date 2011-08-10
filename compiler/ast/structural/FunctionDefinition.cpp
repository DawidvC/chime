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
    
    llvm::Value* FunctionDefinition::createMethod(chime::code_generator& generator, const std::string& name, NodeRef body, unsigned int arity)
    {
        llvm::Function*              methodFunction;
        llvm::Function::arg_iterator args;
        std::string                  functionName;
        llvm::Value*                 functionNameCStringPtr;
        llvm::BasicBlock*            basicBlock;
        llvm::BasicBlock*            currentBlock;
        llvm::Value*                 classObjectPtr;
        llvm::AllocaInst*            alloca;
        
        currentBlock = generator.builder()->GetInsertBlock();
        
        functionName           = generator.getImplementationScope()->getName() + "." + name;
        functionNameCStringPtr = generator.make_constant_string(name);
        
        methodFunction = generator.createFunction(generator.getRuntime()->getChimeFunctionType(), functionName);
        
        // create the return code for the method
        generator.setMethodScope(chime::MethodScopeRef(new chime::MethodScope()));
        
        // setup the function entry
        basicBlock = llvm::BasicBlock::Create(generator.getContext(), "entry", methodFunction, 0);
        generator.builder()->SetInsertPoint(basicBlock);
        
        // deal with the arguments
        args = methodFunction->arg_begin();
        
        alloca = generator.insert_chime_object_alloca();
        
        generator.builder()->CreateStore(args++, alloca, false);
        
        generator.getMethodScope()->setSelfPointer(alloca);
        
        // create the actual method body
        body->codegen(generator);
        
        // only do this if we haven't yet terminated the block
        if (!generator.builder()->GetInsertBlock()->getTerminator())
        {
            generator.builder()->CreateRet(generator.getRuntime()->getChimeLiteralNull());
        }
        
        // before we continue, verify the function
        llvm::verifyFunction(*methodFunction);
        
        // restore the builder's position
        generator.builder()->SetInsertPoint(currentBlock);
        
        // get the class object
        classObjectPtr = generator.getImplementationScope()->getTarget();
        
        // finally, install the method in the class
        generator.getRuntime()->callChimeObjectSetFunction(classObjectPtr, functionNameCStringPtr, methodFunction, arity);
        
        return NULL;
    }
}
