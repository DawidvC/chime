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
    
    void FunctionDefinition::codegenMethodParameters(chime::code_generator& generator, llvm::Function* function)
    {
        unsigned int                 i;
        ast::method_parameter*       param;
        llvm::Function::arg_iterator args;
        llvm::AllocaInst*            alloca;
        
        // first, deal with self
        args = function->arg_begin();
        
        alloca = generator.insertChimeObjectAlloca();
        generator.builder()->CreateStore(args, alloca, false);
        
        generator.getCurrentScope()->setValueForIdentifier("_self", alloca);
        
        // Careful here.  Properties define two methods, one that has params and one that does not.  The
        // safest thing to do here is to iterate over the function arguments, and fill in as we go
        ++args; // advance past the self argument
        
        for (i = 0; args != function->arg_end(); ++args, ++i)
        {
            param = static_cast<ast::method_parameter*>(_parameters->childAtIndex(i));
            
            alloca = generator.insert_chime_object_alloca();
            generator.builder()->CreateStore(args, alloca, false);
            generator.getCurrentScope()->setValueForIdentifier(param->identifier(), alloca);
        }
    }
    
    llvm::Value* FunctionDefinition::createMethod(chime::code_generator& generator, const std::string& name, NodeRef body, unsigned int arity)
    {
        llvm::Function*                methodFunction;
        std::string                    functionName;
        llvm::Value*                   functionNameCStringPtr;
        llvm::BasicBlock*              basicBlock;
        llvm::BasicBlock*              currentBlock;
        llvm::Value*                   classObjectPtr;
        std::vector<const llvm::Type*> functionArgs;
        llvm::FunctionType*            functionType;
        
        // setup the function type
        functionArgs.push_back(generator.getRuntime()->getChimeObjectPtrType()); // this is self
        
        // add another object pointer for every argument the function takes
        for (int i = 0; i < arity; ++i)
        {
            functionArgs.push_back(generator.getRuntime()->getChimeObjectPtrType());
        }
        
        functionType = llvm::FunctionType::get(generator.getRuntime()->getChimeObjectPtrType(), functionArgs, false);
        
        // setup for creating the function itself
        
        currentBlock = generator.builder()->GetInsertBlock();
        
        functionName           = generator.getImplementationScope()->getName() + "." + name;
        functionNameCStringPtr = generator.make_constant_string(name);
        
        methodFunction = generator.createFunction(functionType, functionName);
        
        // create the return code for the method
        generator.pushScope(this);
        
        // setup the function entry
        basicBlock = llvm::BasicBlock::Create(generator.getContext(), "entry", methodFunction, 0);
        generator.builder()->SetInsertPoint(basicBlock);
        
        // deal with the arguments
        this->codegenMethodParameters(generator, methodFunction);
        
        // create the actual method body
        body->codegen(generator);
        
        // only do this if we haven't yet terminated the block
        if (!generator.builder()->GetInsertBlock()->getTerminator())
        {
            generator.builder()->CreateRet(generator.getRuntime()->getChimeLiteralNull());
        }
        
        // before we continue, verify the function
        llvm::verifyFunction(*methodFunction);
        
        // restore the scope and builder's position
        generator.popScope();
        generator.builder()->SetInsertPoint(currentBlock);
        
        // get the class object
        classObjectPtr = generator.getImplementationScope()->getTarget();
        
        // finally, install the method in the class
        generator.getRuntime()->callChimeRuntimeSetInstanceMethod(classObjectPtr, functionNameCStringPtr, methodFunction);
        
        return NULL;
    }
}
