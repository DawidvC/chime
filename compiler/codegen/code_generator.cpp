#include "code_generator.h"

#include <assert.h>

namespace chime
{
    code_generator::code_generator()
    {
        _module               = NULL;
        _builder              = new llvm::IRBuilder<>(llvm::getGlobalContext());
        _importedNamespaces   = new std::vector<std::string>();
        
        _currentScope         = NULL;
        
        _object_ptr_type     = NULL;
        _c_string_ptr_type   = NULL;
        _chime_function_type = NULL;
        
        _runtimeInterface    = NULL;
    }
    
    code_generator::~code_generator()
    {
        delete _builder;
        delete _importedNamespaces;
        
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
    
    llvm::Value* code_generator::getConstantString(std::string str)
    {
        llvm::ArrayType*             string_array_type;
        llvm::GlobalVariable*        global_string;
        llvm::ConstantInt*           const_int32;
        std::vector<llvm::Constant*> const_ptr_indices;
        llvm::Constant*              const_ptr;
        llvm::Constant*              const_c_string_array;
        
        string_array_type = llvm::ArrayType::get(llvm::IntegerType::get(this->getContext(), 8), str.length()+1);
        
        global_string = new llvm::GlobalVariable(*(this->module()), string_array_type, true, llvm::GlobalValue::PrivateLinkage, 0, ".str");
        
        const_int32 = llvm::ConstantInt::get(this->getContext(), llvm::APInt(32, llvm::StringRef("0"), 10));
        
        const_ptr_indices.push_back(const_int32);
        const_ptr_indices.push_back(const_int32);
        
        const_ptr = llvm::ConstantExpr::getGetElementPtr(global_string, &const_ptr_indices[0], const_ptr_indices.size());
        
        const_c_string_array = llvm::ConstantArray::get(this->getContext(), str.c_str(), true);
        
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
        llvm::Value*    condCompareToNull;
        llvm::Value*    condCompareToFalse;
        llvm::Constant* falseConstant;
        llvm::Constant* nullConstant;
        llvm::Value*    condition;
        llvm::Value*    loadedObjectPtr;
        
        loadedObjectPtr = this->builder()->CreateLoad(cond, "value being compared");
        
        nullConstant  = this->getRuntime()->getChimeLiteralNull();
        falseConstant = this->getRuntime()->getChimeLiteralFalse();
        
        // compare the value to to the false literal
        // compare the value to to the null literal
        condCompareToFalse = this->builder()->CreateICmpEQ(loadedObjectPtr, falseConstant);
        condCompareToNull  = this->builder()->CreateICmpEQ(loadedObjectPtr, nullConstant);
        
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
    
    void code_generator::generateMainFunction()
    {
        llvm::LLVMContext&             context = this->module()->getContext();
        const llvm::IntegerType*       int32_type;
        llvm::Type*                    int8_ptr_type;
        std::vector<const llvm::Type*> functionArgs;
        llvm::FunctionType*            functionType; 
        llvm::Function*                function;
        llvm::BasicBlock*              block;
        
        // create the main function
        int32_type    = llvm::IntegerType::get(context, 32);
        int8_ptr_type = this->get_c_string_ptr_type();
        
        functionArgs.push_back(int32_type);
        functionArgs.push_back(llvm::PointerType::get(int8_ptr_type, 0));
        
        functionType = llvm::FunctionType::get(int32_type, functionArgs, false);
        
        function = this->createFunction(functionType, std::string("main"));
        
        // function body
        block = llvm::BasicBlock::Create(context, "entry", function, 0);
        this->builder()->SetInsertPoint(block);
        
        this->getRuntime()->callChimeRuntimeInitialize();
        this->getRuntime()->callChimeLibraryInitialize();
    }
    
    void code_generator::generateModuleInitFunction(const std::string& moduleName)
    {
        llvm::Function*   function;
        llvm::BasicBlock* block;
        
        function = this->getRuntime()->createModuleInitFunction(moduleName);
        
        block = llvm::BasicBlock::Create(this->getContext(), "entry", function, 0);
        this->builder()->SetInsertPoint(block);
    }
    
    void code_generator::generate(ast::Root* node, const std::string& moduleName, bool asMain)
    {
        assert(node);
        
        this->pushScope(node);
        
        _module = new llvm::Module(moduleName, llvm::getGlobalContext());
        
        _runtimeInterface = new RuntimeInterface(this->module(), this->builder());
        
        if (asMain)
        {
            this->generateMainFunction();
        }
        else
        {
            this->generateModuleInitFunction(moduleName);
        }
        
        // actually do the codegen
        node->codegen(*this);
        
        if (asMain)
        {
            // by default, return a zero
            this->builder()->CreateRet(llvm::ConstantInt::get(this->getContext(), llvm::APInt(32, 0, false)));
        }
        else
        {
            this->builder()->CreateRetVoid();
        }
        
        llvm::verifyModule(*this->module(), llvm::PrintMessageAction);
    }
}
