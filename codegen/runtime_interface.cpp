#include "codegen/runtime_interface.h"

namespace chime
{
    RuntimeInterface::RuntimeInterface(llvm::Module* module, llvm::IRBuilder<>* builder)
    {
        _module            = module;
        _builder           = builder;
        
        _objectPtrType     = NULL;
        _chimeFunctionType = NULL;
        
        _functionChimeRuntimeInitialize       = NULL;
        _functionChimeLibraryInitialize       = NULL;
        _functionChimeRuntimeCreateClass      = NULL;
        _functionChimeRuntimeGetClass         = NULL;
        _functionChimeRuntimeLoad             = NULL;
        _functionChimeObjectCreate            = NULL;
        _functionChimeObjectSetFunction       = NULL;
        _functionChimeObjectInvoke            = NULL;
        _functionChimeLiteralEncodeInteger    = NULL;
        _functionChimeLiteralEncodeBoolean    = NULL;
        _functionChimeStringCreateWithCString = NULL;
        
        _literalNull = NULL;
    }
    
    RuntimeInterface::~RuntimeInterface()
    {
    }
    
#pragma mark -
#pragma mark Accessors
    llvm::Module* RuntimeInterface::getModule(void) const
    {
        return _module;
    }
    
    llvm::LLVMContext& RuntimeInterface::getContext(void) const
    {
        return _module->getContext();
    }
    
    llvm::IRBuilder<>* RuntimeInterface::getBuilder(void) const
    {
        return _builder;
    }

#pragma mark -
#pragma mark Basic Types
    llvm::Type* RuntimeInterface::getCStringPtrType(void)
    {
        return llvm::PointerType::get(llvm::IntegerType::get(this->getContext(), 8), 0);
    }

#pragma mark -
#pragma mark Types
    llvm::Type* RuntimeInterface::getChimeObjectPtrType(void)
    {
        llvm::OpaqueType* objectStructType;
        
        if (_objectPtrType)
            return _objectPtrType;
            
        objectStructType = llvm::OpaqueType::get(this->getContext());
        
        this->getModule()->addTypeName("struct._chime_object", objectStructType);
         
        _objectPtrType = llvm::PointerType::get(objectStructType, 0);
        
        return _objectPtrType;
    }
    
    llvm::FunctionType* RuntimeInterface::getChimeFunctionType(void)
    {
        std::vector<const llvm::Type*> functionArgs;
        
        if (_chimeFunctionType)
            return _chimeFunctionType;
        
        functionArgs.push_back(this->getChimeObjectPtrType());
        functionArgs.push_back(this->getCStringPtrType());
        
        _chimeFunctionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, true);
        
        return _chimeFunctionType;
    }
    
    llvm::FunctionType* RuntimeInterface::getChimeModuleInitFunctionType(void)
    {
        std::vector<const llvm::Type*> functionArgs;
        
        return llvm::FunctionType::get(llvm::Type::getVoidTy(this->getContext()), functionArgs, false);
    }
    
#pragma mark -
#pragma mark Runtime Functions
    void RuntimeInterface::callChimeRuntimeInitialize(void)
    {
        if (_functionChimeRuntimeInitialize == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionType = llvm::FunctionType::get(llvm::Type::getVoidTy(this->getContext()), functionArgs, false);
            
            _functionChimeRuntimeInitialize = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_runtime_initialize", this->getModule());
            _functionChimeRuntimeInitialize->setCallingConv(llvm::CallingConv::C);
        }
        
        this->getBuilder()->CreateCall(_functionChimeRuntimeInitialize, "");
    }
    
    void RuntimeInterface::callChimeLibraryInitialize(void)
    {
        if (_functionChimeLibraryInitialize == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionType = llvm::FunctionType::get(llvm::Type::getVoidTy(this->getContext()), functionArgs, false);
            
            _functionChimeLibraryInitialize = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_library_initialize", this->getModule());
            _functionChimeLibraryInitialize->setCallingConv(llvm::CallingConv::C);
        }
        
        this->getBuilder()->CreateCall(_functionChimeLibraryInitialize, "");
    }
    
    llvm::Value* RuntimeInterface::callChimeRuntimeCreateClass(llvm::Value* classNamePtr, llvm::Value* superclassObjectPtr)
    {
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        if (_functionChimeRuntimeCreateClass == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getCStringPtrType());
            functionArgs.push_back(this->getChimeObjectPtrType());
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeRuntimeCreateClass = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_runtime_create_class", this->getModule());
            _functionChimeRuntimeCreateClass->setCallingConv(llvm::CallingConv::C);
        }
        
        llvm::LoadInst*           loadedObjectPtr;
        std::vector<llvm::Value*> args;
        
        loadedObjectPtr = this->getBuilder()->CreateLoad(superclassObjectPtr, "instance of superclass");
        
        args.push_back(classNamePtr);
        args.push_back(loadedObjectPtr);
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "superclass object pointer");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(_functionChimeRuntimeCreateClass, args.begin(), args.end(), "create class");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeRuntimeGetClass(llvm::Value* classNamePtr)
    {
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        if (_functionChimeRuntimeGetClass == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getCStringPtrType());
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeRuntimeGetClass = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_runtime_get_class", this->getModule());
            _functionChimeRuntimeGetClass->setCallingConv(llvm::CallingConv::C);
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "class object pointer");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(_functionChimeRuntimeGetClass, classNamePtr, "class lookup");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeRuntimeLoad(llvm::Value* namePtr)
    {
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        if (_functionChimeRuntimeLoad == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getCStringPtrType());
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeRuntimeLoad = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_runtime_load", this->getModule());
            _functionChimeRuntimeLoad->setCallingConv(llvm::CallingConv::C);
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "load result");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(_functionChimeRuntimeLoad, namePtr, "load");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
#pragma mark -
#pragma mark Object Functions
    llvm::Value* RuntimeInterface::callChimeObjectCreate(llvm::Value* classPtr)
    {
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        llvm::LoadInst*   loadedObjectPtr;
        
        if (_functionChimeObjectCreate == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getChimeObjectPtrType());
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeObjectCreate = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_object_create", this->getModule());
            _functionChimeObjectCreate->setCallingConv(llvm::CallingConv::C);
        }
        
        loadedObjectPtr = this->getBuilder()->CreateLoad(classPtr, "class instance for object create");
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "instance pointer");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(_functionChimeObjectCreate, loadedObjectPtr, "Object.new");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    void RuntimeInterface::callChimeObjectSetFunction(llvm::Value* objectValue, llvm::Value* propertyNamePtr, llvm::Function* function, unsigned int arity)
    {
        llvm::CallInst* call;
        
        if (_functionChimeObjectSetFunction == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getChimeObjectPtrType());
            functionArgs.push_back(this->getCStringPtrType());
            functionArgs.push_back(llvm::PointerType::get(this->getChimeFunctionType(), 0));
            functionArgs.push_back(llvm::IntegerType::get(this->getContext(), 64));
            
            functionType = llvm::FunctionType::get(llvm::Type::getVoidTy(this->getContext()), functionArgs, false);
            
            _functionChimeObjectSetFunction = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_object_set_function", this->getModule());
            _functionChimeObjectSetFunction->setCallingConv(llvm::CallingConv::C);
        }
        
        llvm::LoadInst*           loadedObjectPtr;
        std::vector<llvm::Value*> args;
        
        loadedObjectPtr = this->getBuilder()->CreateLoad(objectValue, "instance for object set function");
        
        args.push_back(loadedObjectPtr);
        args.push_back(propertyNamePtr);
        args.push_back(function);
        args.push_back(llvm::ConstantInt::get(this->getContext(), llvm::APInt(64, arity, 10)));
        
        call = this->getBuilder()->CreateCall(_functionChimeObjectSetFunction, args.begin(), args.end(), "");
        call->setTailCall(false);
    }
    
    llvm::Value* RuntimeInterface::callChimeObjectInvoke(llvm::Value* objectValue, llvm::Value* namePtr, std::vector<llvm::Value*> args)
    {
        llvm::CallInst*                     call;
        std::vector<llvm::Value*>::iterator it;
        llvm::AllocaInst*                   alloca;
        
        if (_functionChimeObjectInvoke == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getChimeObjectPtrType());
            functionArgs.push_back(this->getCStringPtrType());
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, true);
            
            _functionChimeObjectInvoke = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_object_invoke", this->getModule());
            _functionChimeObjectInvoke->setCallingConv(llvm::CallingConv::C);
        }
        
        llvm::LoadInst* loadedObjectPtr;
        
        loadedObjectPtr = this->getBuilder()->CreateLoad(objectValue, "instance for object invoke");
        
        // grab an iterator and do some insertions such that
        // the object comes first, followed by the property, followed
        // by whatever arguments were supplied
        it = args.begin();
        it = args.insert(it, namePtr);
        args.insert(it, loadedObjectPtr);
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "object invoke return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(_functionChimeObjectInvoke, args.begin(), args.end(), "object invoke");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
#pragma mark -
#pragma mark Literal Functions
    llvm::Value* RuntimeInterface::callChimeLiteralEncodeInteger(signed long value)
    {
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        if (_functionChimeLiteralEncodeInteger == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(llvm::IntegerType::get(this->getContext(), 64));
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeLiteralEncodeInteger = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_literal_encode_integer", this->getModule());
            _functionChimeLiteralEncodeInteger->setCallingConv(llvm::CallingConv::C);
        }
        
        llvm::Value* integerValue;
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "encode integer return");
        alloca->setAlignment(8);
        
        integerValue = llvm::ConstantInt::get(this->getContext(), llvm::APInt(64, value, 10));
        
        call = this->getBuilder()->CreateCall(_functionChimeLiteralEncodeInteger, integerValue, "encode integer");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeLiteralEncodeBoolean(unsigned char value)
    {
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        if (_functionChimeLiteralEncodeBoolean == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(llvm::IntegerType::get(this->getContext(), 8));
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeLiteralEncodeBoolean = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_literal_encode_boolean", this->getModule());
            _functionChimeLiteralEncodeBoolean->setCallingConv(llvm::CallingConv::C);
        }
        
        llvm::Value* booleanValue;
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "encode boolean return");
        alloca->setAlignment(8);
        
        booleanValue = llvm::ConstantInt::get(this->getContext(), llvm::APInt(8, value, 10));
        
        call = this->getBuilder()->CreateCall(_functionChimeLiteralEncodeBoolean, booleanValue, "encode boolean");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeStringCreateWithCString(llvm::Value* cStringPtr)
    {
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        if (_functionChimeStringCreateWithCString == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getCStringPtrType());
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeStringCreateWithCString = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_string_create_with_c_string", this->getModule());
            _functionChimeStringCreateWithCString->setCallingConv(llvm::CallingConv::C);
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "create string with c string");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(_functionChimeStringCreateWithCString, cStringPtr, "create string");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::getChimeLiteralNull(void)
    {
        if (_literalNull == NULL)
        {
            _literalNull = llvm::ConstantPointerNull::get(static_cast<llvm::PointerType*>(this->getChimeObjectPtrType()));
        }
        
        return _literalNull;
    }
}
