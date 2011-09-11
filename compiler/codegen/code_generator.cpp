#include "code_generator.h"

#include <assert.h>

namespace chime
{
    code_generator::code_generator()
    {
        _module               = NULL;
        _builder              = new llvm::IRBuilder<>(llvm::getGlobalContext());
        _importedNamespaces   = new std::vector<std::string>();
        _initFunctions        = new std::vector<llvm::Function*>();
        _internalInitFunction = NULL;
        
        _currentScope         = NULL;
        
        _object_ptr_type     = NULL;
        _c_string_ptr_type   = NULL;
        _chime_function_type = NULL;
        
        _runtimeInterface    = NULL;
        
        _implementationScope = ImplementationScopeRef();
    }
    
    code_generator::~code_generator()
    {
        delete _builder;
        delete _importedNamespaces;
        delete _initFunctions;
        
        if (_module)
            delete _module;
        
        if (_runtimeInterface)
            delete _runtimeInterface;
    }
    
    llvm::IRBuilder<>* code_generator::builder(void) const
    {
        return _builder;
    }
    
    llvm::Module* code_generator::module(void) const
    {
        return _module;
    }
    
    llvm::LLVMContext& code_generator::getContext(void) const
    {
        return _module->getContext();
    }
    
    llvm::LLVMContext& code_generator::get_context(void) const
    {
        return this->getContext();
    }
    
    RuntimeInterface* code_generator::getRuntime(void) const
    {
        return _runtimeInterface;
    }
    
    std::vector<std::string>* code_generator::getImportedNamespaces(void) const
    {
        return _importedNamespaces;
    }
    
    ast::ScopedNode* code_generator::getCurrentScope() const
    {
        return _currentScope;
    }
    
    void code_generator::setCurrentScope(ast::ScopedNode* node)
    {
        _currentScope = node;
    }
    
    void code_generator::pushScope(ast::ScopedNode* scope)
    {
        if (!_currentScope)
        {
            _currentScope = scope;
            return;
        }
        
        scope->setParent(_currentScope);
        
        _currentScope = scope;
    }
    
    void code_generator::popScope()
    {
        assert(_currentScope);
        assert(_currentScope->getParent());
        
        _currentScope = _currentScope->getParent();
    }
    
    ImplementationScopeRef code_generator::getImplementationScope(void) const
    {
        return _implementationScope;
    }
    void code_generator::setImplementationScope(ImplementationScopeRef scope)
    {
        _implementationScope = scope;
    }
    
    std::vector<llvm::Function*>* code_generator::getInitFunctions(void) const
    {
        return _initFunctions;
    }
    
    llvm::Value* code_generator::getConstantString(std::string str)
    {
        llvm::ArrayType*             string_array_type;
        llvm::GlobalVariable*        global_string;
        llvm::ConstantInt*           const_int32;
        std::vector<llvm::Constant*> const_ptr_indices;
        llvm::Constant*              const_ptr;
        llvm::Constant*              const_c_string_array;
        
        string_array_type = llvm::ArrayType::get(llvm::IntegerType::get(this->get_context(), 8), str.length()+1);
        
        global_string = new llvm::GlobalVariable(*(this->module()), string_array_type, true, llvm::GlobalValue::PrivateLinkage, 0, ".str");
        
        const_int32 = llvm::ConstantInt::get(this->get_context(), llvm::APInt(32, llvm::StringRef("0"), 10));
        
        const_ptr_indices.push_back(const_int32);
        const_ptr_indices.push_back(const_int32);
        
        const_ptr = llvm::ConstantExpr::getGetElementPtr(global_string, &const_ptr_indices[0], const_ptr_indices.size());
        
        const_c_string_array = llvm::ConstantArray::get(this->get_context(), str.c_str(), true);
        
        global_string->setInitializer(const_c_string_array);
        
        return const_ptr;
    }
    
    llvm::Value* code_generator::make_constant_string(std::string str)
    {
        return this->getConstantString(str);
    }
    
    llvm::Type* code_generator::get_c_string_ptr_type(void)
    {
        if (!_c_string_ptr_type)
            _c_string_ptr_type = llvm::PointerType::get(llvm::IntegerType::get(this->get_context(), 8), 0);
        
        return _c_string_ptr_type;
    }
    
    llvm::AllocaInst* code_generator::insertChimeObjectAlloca(void)
    {
        llvm::AllocaInst* alloca;
        
        alloca = this->builder()->CreateAlloca(this->getRuntime()->getChimeObjectPtrType(), 0, "object pointer");
        alloca->setAlignment(8);
        
        return alloca;
    }
    
    llvm::AllocaInst* code_generator::insert_chime_object_alloca(void)
    {
        return this->insertChimeObjectAlloca();
    }
    
    llvm::Value* code_generator::createCondition(llvm::Value* cond)
    {
        llvm::Value* condCompareToNull;
        llvm::Value* condCompareToFalse;
        llvm::Constant* falseConstant;
        llvm::Constant* zeroConstant;
        llvm::Value* condition;
        llvm::Constant* zeroValue;
        llvm::Constant* falseValue;
        llvm::Value* loadedObjectPtr;
        
        loadedObjectPtr = this->builder()->CreateLoad(cond, "value being compared");
        
        zeroValue  = llvm::ConstantInt::get(this->get_context(), llvm::APInt(32, 0));
        falseValue = llvm::ConstantInt::get(this->get_context(), llvm::APInt(32, 10));
        
        zeroConstant  = llvm::ConstantExpr::getCast(llvm::Instruction::IntToPtr, zeroValue, this->getRuntime()->getChimeObjectPtrType());
        falseConstant = llvm::ConstantExpr::getCast(llvm::Instruction::IntToPtr, falseValue, this->getRuntime()->getChimeObjectPtrType());
        
        // compare the value to to the false literal
        // compare the value to to the null literal
        condCompareToFalse = this->builder()->CreateICmpEQ(loadedObjectPtr, falseConstant);
        condCompareToNull  = this->builder()->CreateICmpEQ(loadedObjectPtr, zeroConstant);
        
        // or those two comparisons together
        condition = this->builder()->CreateOr(condCompareToFalse, condCompareToNull);
        
        // both of these comparisons need to fail for this to be condition to be taken
        return this->builder()->CreateICmpNE(condition, llvm::ConstantInt::get(this->get_context(), llvm::APInt(1, 1)));
    }
    
    llvm::Function* code_generator::createFunction(const llvm::FunctionType* type, const std::string name)
    {
        llvm::Function* function;
        
        function = llvm::Function::Create(type, llvm::GlobalValue::ExternalLinkage, name, this->module());
        function->setCallingConv(llvm::CallingConv::C);
        
        return function;
    }
    
    llvm::Value* code_generator::callModuleInitFunction(const std::string& name)
    {
        llvm::Function*     initFunction;
        llvm::FunctionType* functionType;
        std::string         functionName;
        
        functionName = "init_" + name;
        
        functionType = this->getRuntime()->getChimeModuleInitFunctionType();
        
        initFunction = this->createFunction(functionType, functionName);
        
        // If there was already a function defined, llvm will automatically rename the
        // new one.  We need to check here to make sure that didn't happen
        // fprintf(stderr, "name = %s f->getName = %s\n", name.c_str(), initFunction->getName().c_str());
        // if (initFunction->getName() != name)
        // {
        //     assert(0 && "This case needs to be handled");
        // }
        
        llvm::CallInst* call;
        
        call = this->builder()->CreateCall(initFunction, "");
        call->setTailCall(false);
        
        return NULL;
    }
    
    void code_generator::generateMainFunction(void)
    {
        llvm::LLVMContext&             context = this->module()->getContext();
        const llvm::IntegerType*       int32_type;
        llvm::Type*                    int8_ptr_type;
        std::vector<const llvm::Type*> functionArgs;
        llvm::FunctionType*            functionType; 
        llvm::Function*                function;
        
        // create the main function
        int32_type    = llvm::IntegerType::get(context, 32);
        int8_ptr_type = this->get_c_string_ptr_type();
        
        functionArgs.push_back(int32_type);
        functionArgs.push_back(llvm::PointerType::get(int8_ptr_type, 0));
        
        functionType = llvm::FunctionType::get(int32_type, functionArgs, false);
        
        function = this->createFunction(functionType, std::string("main"));
        
        // function body
        llvm::BasicBlock* label_entry = llvm::BasicBlock::Create(context, "entry", function, 0);
        this->builder()->SetInsertPoint(label_entry);
        
        this->getRuntime()->callChimeRuntimeInitialize();
        this->getRuntime()->callChimeLibraryInitialize();
        
        // now, create our internal init function and call it.  This function is
        // really just used as a placeholder we'll fill in after codegen
        // with compiled and/or linked implemenations
        
        functionArgs.clear();
        
        functionType = llvm::FunctionType::get(llvm::Type::getVoidTy(this->getContext()), functionArgs, false);
        
        _internalInitFunction = this->createFunction(functionType, "internal_initialize");
        
        this->builder()->CreateCall(_internalInitFunction, "");
    }
    
    void code_generator::fillInInternalInitFunction(void)
    {
        llvm::BasicBlock*                      basicBlock;
        llvm::BasicBlock*                      currentBlock;
        std::vector<llvm::Function*>::iterator i;
        
        assert(_internalInitFunction && "_internalInitFunction cannot be NULL");
        
        // capture the current block
        currentBlock = this->builder()->GetInsertBlock();
        
        // setup our insertion point in the init function
        basicBlock = llvm::BasicBlock::Create(this->getContext(), "entry", _internalInitFunction, 0);
        this->builder()->SetInsertPoint(basicBlock);
        
        for (i = _initFunctions->begin(); i < _initFunctions->end(); ++i)
        {
            this->builder()->CreateCall((*i), "");
        }
        
        this->builder()->CreateRetVoid();
        
        // verify the function
        llvm::verifyFunction(*_internalInitFunction);
        
        // restore the builder's position
        this->builder()->SetInsertPoint(currentBlock);
    }
    
    void code_generator::generate(ast::Root* node, const std::string& moduleName, bool asMain)
    {
        std::vector<ast::node*>::iterator i;
        
        assert(node != NULL);
        
        this->pushScope(node);
        
        _module = new llvm::Module(moduleName, llvm::getGlobalContext());
        
        _runtimeInterface = new RuntimeInterface(this->module(), this->builder());
        
        if (asMain)
        {
            this->generateMainFunction();
        }
        else
        {
            llvm::Function*   function;
            llvm::BasicBlock* labelEntry;
            
            function = this->getRuntime()->createModuleInitFunction(moduleName);
            
            labelEntry = llvm::BasicBlock::Create(this->getContext(), "entry", function, 0);
            this->builder()->SetInsertPoint(labelEntry);
        }
        
        // actually do the codegen
        for (i=node->children()->begin(); i < node->children()->end(); ++i)
        {
            (*i)->codegen(*this);
        }
        
        if (asMain)
        {
            // by default, return a zero
            this->builder()->CreateRet(llvm::ConstantInt::get(this->get_context(), llvm::APInt(32, 0, false)));
            
            // and now, generate all of the internal init calls
            this->fillInInternalInitFunction();
        }
        else
        {
            this->builder()->CreateRetVoid();
        }
        
        llvm::verifyModule(*this->module(), llvm::PrintMessageAction);
    }
}
