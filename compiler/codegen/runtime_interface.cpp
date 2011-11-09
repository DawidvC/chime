#include "runtime_interface.h"

namespace chime
{
    RuntimeInterface::RuntimeInterface(llvm::Module* module, llvm::IRBuilder<>* builder)
    {
        _module            = module;
        _builder           = builder;
        
        _objectPtrType     = NULL;
        _objectPtrPtrType  = NULL;
        _chimeFunctionType = NULL;
        _closurePtrType    = NULL;
        
        _functionChimeRuntimeInitialize        = NULL;
        _functionChimeLibraryInitialize        = NULL;
        _functionChimeRuntimeCreateClass       = NULL;
        _functionChimeRuntimeGetClass          = NULL;
        _functionChimeRuntimeGetTrait          = NULL;
        _functionChimeRuntimeLoad              = NULL;
        _functionChimeObjectCreate             = NULL;
        _functionChimeRuntimeSetInstanceMethod = NULL;
        _functionChimeRuntimeSetClassMethod    = NULL;
        _functionChimeClassIncludeTrait        = NULL;
        _functionChimeTraitCreate              = NULL;
        _functionChimeObjectGetAttribute       = NULL;
        _functionChimeObjectSetAttribute       = NULL;
        _functionChimeObjectInvoke0            = NULL;
        _functionChimeObjectInvoke1            = NULL;
        _functionChimeObjectInvoke2            = NULL;
        _functionChimeObjectInvoke3            = NULL;
        _functionChimeLiteralEncodeInteger     = NULL;
        _functionChimeLiteralEncodeBoolean     = NULL;
        _functionChimeStringCreateWithCString  = NULL;
        _functionChimeRangeCreate              = NULL;
        _functionChimeClosureCreate            = NULL;
        _functionChimeReferenceCreate          = NULL;
        _functionChimeReferenceGet             = NULL;
        _functionChimeReferenceSet             = NULL;
        _functionChimeArrayCreateWithLength    = NULL;
        _functionChimeArrayAppend              = NULL;
        _functionChimeFloatCreate              = NULL;
        _functionChimeHashCreate               = NULL;
        _functionChimeHashSet                  = NULL;
        
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
    llvm::Type* RuntimeInterface::getVoidPtrType(void)
    {
        return llvm::PointerType::get(llvm::IntegerType::get(this->getContext(), 8), 0);
    }
    
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
        
        this->getModule()->addTypeName("chime_object_t", objectStructType);
         
        _objectPtrType = llvm::PointerType::get(objectStructType, 0);
        
        return _objectPtrType;
    }
    
    llvm::Type* RuntimeInterface::getChimeObjectPtrPtrType(void)
    {
        if (_objectPtrPtrType)
            return _objectPtrPtrType;
        
        _objectPtrPtrType = llvm::PointerType::get(this->getChimeObjectPtrType(), 0);
        
        return _objectPtrPtrType;
    }
    
    llvm::Type* RuntimeInterface::getChimeClassPtrType(void)
    {
        llvm::OpaqueType* classStructType;
        
        if (_classPtrType)
            return _classPtrType;
            
        classStructType = llvm::OpaqueType::get(this->getContext());
        
        this->getModule()->addTypeName("struct._chime_class", classStructType);
         
        _classPtrType = llvm::PointerType::get(classStructType, 0);
        
        return _classPtrType;
    }
    
    llvm::Type* RuntimeInterface::getChimeClosurePtrType(void)
    {
        llvm::OpaqueType* closureStructType;
        
        if (_closurePtrType)
            return _closurePtrType;
            
        closureStructType = llvm::OpaqueType::get(this->getContext());
        
        this->getModule()->addTypeName("struct._chime_closure", closureStructType);
         
        _closurePtrType = llvm::PointerType::get(closureStructType, 0);
        
        return _closurePtrType;
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
    
    llvm::Value* RuntimeInterface::callChimeRuntimeGetTrait(llvm::Value* traitNamePtr)
    {
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        if (_functionChimeRuntimeGetTrait == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getCStringPtrType());
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeRuntimeGetTrait = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_runtime_get_trait", this->getModule());
            _functionChimeRuntimeGetTrait->setCallingConv(llvm::CallingConv::C);
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_runtime_get_trait: return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(_functionChimeRuntimeGetTrait, traitNamePtr, "chime_runtime_get_trait call");
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
    
    llvm::Function* RuntimeInterface::createModuleInitFunction(const std::string& name)
    {
        std::vector<const llvm::Type*> functionArgs;
        llvm::FunctionType*            functionType;
        llvm::Function*                function;
        
        functionType = llvm::FunctionType::get(llvm::Type::getVoidTy(this->getContext()), functionArgs, false);
        
        function = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "init_" + name, this->getModule());
        function->setCallingConv(llvm::CallingConv::C);
        
        return function;
    }
    
    llvm::Value* RuntimeInterface::callModuleInitFunction(const std::string& name)
    {
        llvm::Function*     initFunction;
        llvm::FunctionType* functionType;
        std::string         functionName;
        llvm::CallInst*     call;
        
        functionName = "init_" + name;
        
        functionType = this->getChimeModuleInitFunctionType();
        
        initFunction = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, functionName, this->getModule());
        initFunction->setCallingConv(llvm::CallingConv::C);
        
        call = this->getBuilder()->CreateCall(initFunction, "");
        call->setTailCall(false);
        
        return NULL;
    }
    
    void RuntimeInterface::callChimeRuntimeSetInstanceMethod(llvm::Value* classValue, llvm::Value* functionNamePtr, llvm::Function* function)
    {
        llvm::CallInst*           call;
        llvm::LoadInst*           loadedObjectPtr;
        std::vector<llvm::Value*> args;
        llvm::Constant*           functionPtr;
        
        if (_functionChimeRuntimeSetInstanceMethod == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getChimeObjectPtrType());
            functionArgs.push_back(this->getCStringPtrType());
            functionArgs.push_back(this->getVoidPtrType());
            
            functionType = llvm::FunctionType::get(llvm::Type::getVoidTy(this->getContext()), functionArgs, false);
            
            _functionChimeRuntimeSetInstanceMethod = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_runtime_set_instance_method", this->getModule());
            _functionChimeRuntimeSetInstanceMethod->setCallingConv(llvm::CallingConv::C);
        }
        
        loadedObjectPtr = this->getBuilder()->CreateLoad(classValue, "instance for object set function");
        
        functionPtr = llvm::ConstantExpr::getCast(llvm::Instruction::BitCast, function, this->getVoidPtrType());
        
        args.push_back(loadedObjectPtr);
        args.push_back(functionNamePtr);
        args.push_back(functionPtr);
        
        call = this->getBuilder()->CreateCall(_functionChimeRuntimeSetInstanceMethod, args.begin(), args.end(), "");
        call->setTailCall(false);
    }
    
    void RuntimeInterface::callChimeRuntimeSetClassMethod(llvm::Value* classValue, llvm::Value* functionNamePtr, llvm::Function* function)
    {
        llvm::CallInst*           call;
        llvm::LoadInst*           loadedObjectPtr;
        std::vector<llvm::Value*> args;
        llvm::Constant*           functionPtr;
        
        if (_functionChimeRuntimeSetClassMethod == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getChimeObjectPtrType());
            functionArgs.push_back(this->getCStringPtrType());
            functionArgs.push_back(this->getVoidPtrType());
            
            functionType = llvm::FunctionType::get(llvm::Type::getVoidTy(this->getContext()), functionArgs, false);
            
            _functionChimeRuntimeSetClassMethod = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_runtime_set_class_method", this->getModule());
            _functionChimeRuntimeSetClassMethod->setCallingConv(llvm::CallingConv::C);
        }
        
        loadedObjectPtr = this->getBuilder()->CreateLoad(classValue, "chime_runtime_set_class_method param1:class");
        
        functionPtr = llvm::ConstantExpr::getCast(llvm::Instruction::BitCast, function, this->getVoidPtrType());
        
        args.push_back(loadedObjectPtr);
        args.push_back(functionNamePtr);
        args.push_back(functionPtr);
        
        call = this->getBuilder()->CreateCall(_functionChimeRuntimeSetClassMethod, args.begin(), args.end(), "");
        call->setTailCall(false);
    }

// Class Functions
    void RuntimeInterface::callChimeClassIncludeTrait(llvm::Value* classPtr, llvm::Value* traitPtr)
    {
        llvm::CallInst*           call;
        llvm::LoadInst*           loadedClassPtr;
        llvm::LoadInst*           loadedTraitPtr;
        std::vector<llvm::Value*> args;
        
        if (_functionChimeClassIncludeTrait == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getChimeObjectPtrType());
            functionArgs.push_back(this->getChimeObjectPtrType());
            
            functionType = llvm::FunctionType::get(this->getVoidPtrType(), functionArgs, false);
            
            _functionChimeClassIncludeTrait = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_class_include_trait", this->getModule());
            _functionChimeClassIncludeTrait->setCallingConv(llvm::CallingConv::C);
        }
        
        loadedClassPtr = this->getBuilder()->CreateLoad(classPtr, "chime_class_include_trait param1:class");
        loadedTraitPtr = this->getBuilder()->CreateLoad(traitPtr, "chime_class_include_trait param2:trait");
        
        args.push_back(loadedClassPtr);
        args.push_back(loadedTraitPtr);
        
        call = this->getBuilder()->CreateCall(_functionChimeClassIncludeTrait, args.begin(), args.end(), "");
        call->setTailCall(false);
    }
    
// Trait Functions
    llvm::Value* RuntimeInterface::callChimeTraitCreate(llvm::Value* traitNamePtr)
    {
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        if (_functionChimeTraitCreate == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getCStringPtrType());
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeTraitCreate = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_trait_create", this->getModule());
            _functionChimeTraitCreate->setCallingConv(llvm::CallingConv::C);
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_trait_create: return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(_functionChimeTraitCreate, traitNamePtr, "chime_trait_create call");
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
    
    llvm::Value* RuntimeInterface::callChimeObjectGetAttribute(llvm::Value* objectValue, llvm::Value* attributeNamePtr)
    {
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        if (_functionChimeObjectGetAttribute == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getChimeObjectPtrType());
            functionArgs.push_back(this->getCStringPtrType());
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeObjectGetAttribute = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_object_get_attribute", this->getModule());
            _functionChimeObjectGetAttribute->setCallingConv(llvm::CallingConv::C);
        }
        
        llvm::LoadInst*           loadedObjectPtr;
        std::vector<llvm::Value*> args;
        
        loadedObjectPtr = this->getBuilder()->CreateLoad(objectValue, "instance for object get attribute");
        
        args.push_back(loadedObjectPtr);
        args.push_back(attributeNamePtr);
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "object get attribute return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(_functionChimeObjectGetAttribute, args.begin(), args.end(), "");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    void RuntimeInterface::callChimeObjectSetAttribute(llvm::Value* objectValue, llvm::Value* attributeNamePtr, llvm::Value* attributeValue)
    {
        llvm::CallInst* call;
        
        if (_functionChimeObjectSetAttribute == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getChimeObjectPtrType());
            functionArgs.push_back(this->getCStringPtrType());
            functionArgs.push_back(this->getChimeObjectPtrType());
            
            functionType = llvm::FunctionType::get(llvm::Type::getVoidTy(this->getContext()), functionArgs, false);
            
            _functionChimeObjectSetAttribute = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_object_set_attribute", this->getModule());
            _functionChimeObjectSetAttribute->setCallingConv(llvm::CallingConv::C);
        }
        
        llvm::LoadInst*           loadedObjectPtr;
        llvm::LoadInst*           loadedValuePtr;
        std::vector<llvm::Value*> args;
        
        loadedObjectPtr = this->getBuilder()->CreateLoad(objectValue, "chime_object_set_attribute arg2:instance");
        loadedValuePtr  = this->getBuilder()->CreateLoad(attributeValue, "chime_object_set_attribute arg2:value");
        
        args.push_back(loadedObjectPtr);
        args.push_back(attributeNamePtr);
        args.push_back(loadedValuePtr);
        
        call = this->getBuilder()->CreateCall(_functionChimeObjectSetAttribute, args.begin(), args.end(), "");
        call->setTailCall(false);
    }
    
    llvm::Value* RuntimeInterface::callChimeObjectInvoke(llvm::Value* objectValue, llvm::Value* namePtr, std::vector<llvm::Value*> args)
    {
        llvm::CallInst*                     call;
        std::vector<llvm::Value*>::iterator it;
        llvm::AllocaInst*                   alloca;
        llvm::Function*                     invokeFunction;
        
        if (_functionChimeObjectInvoke0 == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getChimeObjectPtrType());
            functionArgs.push_back(this->getCStringPtrType());
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            // make the 0 arg version
            _functionChimeObjectInvoke0 = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_object_invoke_0", this->getModule());
            _functionChimeObjectInvoke0->setCallingConv(llvm::CallingConv::C);
            
            // 1 arg version
            functionArgs.push_back(this->getChimeObjectPtrType());
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeObjectInvoke1 = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_object_invoke_1", this->getModule());
            _functionChimeObjectInvoke1->setCallingConv(llvm::CallingConv::C);
            
            // 2 arg version
            functionArgs.push_back(this->getChimeObjectPtrType());
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeObjectInvoke2 = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_object_invoke_2", this->getModule());
            _functionChimeObjectInvoke2->setCallingConv(llvm::CallingConv::C);
            
            // 3 arg version
            functionArgs.push_back(this->getChimeObjectPtrType());
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeObjectInvoke3 = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_object_invoke_3", this->getModule());
            _functionChimeObjectInvoke3->setCallingConv(llvm::CallingConv::C);
        }
        
        llvm::LoadInst* loadedObjectPtr;
        
        loadedObjectPtr = this->getBuilder()->CreateLoad(objectValue, "instance for object invoke");
        
        assert(args.size() <= 5 && "A max of 3 arguments + self and name are supported at the moment");
        
        // grab an iterator and do some insertions such that
        // the object comes first, followed by the property, followed
        // by whatever arguments were supplied
        it = args.begin();
        it = args.insert(it, namePtr);
        args.insert(it, loadedObjectPtr);
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "object invoke return");
        alloca->setAlignment(8);
        
        switch (args.size() - 2) // because self and name are always included
        {
            case 0: invokeFunction = _functionChimeObjectInvoke0; break;
            case 1: invokeFunction = _functionChimeObjectInvoke1; break;
            case 2: invokeFunction = _functionChimeObjectInvoke2; break;
            case 3: invokeFunction = _functionChimeObjectInvoke3; break;
            default:
                assert(0 && "More than three arguments unsupported right now!");
                break;
        }
        
        call = this->getBuilder()->CreateCall(invokeFunction, args.begin(), args.end(), "object invoke");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
#pragma mark -
#pragma mark Literal Functions
    llvm::Value* RuntimeInterface::callChimeLiteralEncodeInteger(llvm::Value* integerValue)
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
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "encode integer return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(_functionChimeLiteralEncodeInteger, integerValue, "encode integer");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeLiteralEncodeBoolean(llvm::Value* booleanValue)
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
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "encode boolean return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(_functionChimeLiteralEncodeBoolean, booleanValue, "encode boolean");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeStringCreateWithCString(llvm::Value* cStringPtr, llvm::Value* lengthValue)
    {
        llvm::CallInst*           call;
        llvm::AllocaInst*         alloca;
        std::vector<llvm::Value*> args;
        
        if (_functionChimeStringCreateWithCString == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getCStringPtrType());
            functionArgs.push_back(llvm::IntegerType::get(this->getContext(), 64));
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeStringCreateWithCString = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_string_create_with_c_string", this->getModule());
            _functionChimeStringCreateWithCString->setCallingConv(llvm::CallingConv::C);
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "create string with c string");
        alloca->setAlignment(8);
        
        args.push_back(cStringPtr);
        args.push_back(lengthValue);
        
        call = this->getBuilder()->CreateCall(_functionChimeStringCreateWithCString, args.begin(), args.end(), "create string");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeRangeCreate(llvm::Value* startValue, llvm::Value* endValue)
    {
        llvm::CallInst*           call;
        llvm::AllocaInst*         alloca;
        std::vector<llvm::Value*> args;
        
        if (_functionChimeRangeCreate == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getChimeObjectPtrType());
            functionArgs.push_back(this->getChimeObjectPtrType());
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeRangeCreate = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_range_create", this->getModule());
            _functionChimeRangeCreate->setCallingConv(llvm::CallingConv::C);
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_range_create: return");
        alloca->setAlignment(8);
        
        startValue = this->getBuilder()->CreateLoad(startValue, "chime_range_create: arg1");
        endValue   = this->getBuilder()->CreateLoad(endValue,   "chime_range_create: arg2");
        
        args.push_back(startValue);
        args.push_back(endValue);
        
        call = this->getBuilder()->CreateCall(_functionChimeRangeCreate, args.begin(), args.end(), "chime_range_create: call");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Constant* RuntimeInterface::getChimeLiteralNull(void)
    {
        // if (_literalNull == NULL)
        // {
        //     _literalNull = llvm::ConstantPointerNull::get(static_cast<llvm::PointerType*>(this->getChimeObjectPtrType()));
        // }
        // 
        // return _literalNull;
        
        llvm::Constant* nilValue;
        
        nilValue = llvm::ConstantInt::get(this->getContext(), llvm::APInt(64, 0));
        
        return llvm::ConstantExpr::getCast(llvm::Instruction::IntToPtr, nilValue, this->getChimeObjectPtrType());
    }
    
    llvm::Constant* RuntimeInterface::getChimeLiteralTrue()
    {
        llvm::Constant* numericValue;
        
        numericValue = llvm::ConstantInt::get(this->getContext(), llvm::APInt(64, 6));
        
        return llvm::ConstantExpr::getCast(llvm::Instruction::IntToPtr, numericValue, this->getChimeObjectPtrType());
    }
    
    llvm::Constant* RuntimeInterface::getChimeLiteralFalse()
    {
        llvm::Constant* numericValue;
        
        numericValue = llvm::ConstantInt::get(this->getContext(), llvm::APInt(64, 10));
        
        return llvm::ConstantExpr::getCast(llvm::Instruction::IntToPtr, numericValue, this->getChimeObjectPtrType());
    }
    
#pragma mark -
#pragma mark Closure Functions
    llvm::Value* RuntimeInterface::callChimeClosureCreate(llvm::Function* function)
    {
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        llvm::Type*       voidPtrType;
        
        // a generic pointer (llvm does not support the void* type)
        voidPtrType = llvm::PointerType::get(llvm::IntegerType::get(this->getContext(), 8), 0);
        
        if (_functionChimeClosureCreate == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(voidPtrType);
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeClosureCreate = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_closure_create", this->getModule());
            _functionChimeClosureCreate->setCallingConv(llvm::CallingConv::C);
        }
        
        llvm::Constant* functionPtr;
        
        functionPtr = llvm::ConstantExpr::getCast(llvm::Instruction::BitCast, function, voidPtrType);
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "new closure");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(_functionChimeClosureCreate, functionPtr, "create closure");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
// Reference Functions
    llvm::Value* RuntimeInterface::callChimeReferenceCreate(llvm::Value* objectValue)
    {
        llvm::CallInst*           call;
        llvm::AllocaInst*         alloca;
        llvm::LoadInst*           load;
        std::vector<llvm::Value*> args;
        
        assert(objectValue);
        
        if (_functionChimeReferenceCreate == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getChimeObjectPtrType());
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeReferenceCreate = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_reference_create", this->getModule());
            _functionChimeReferenceCreate->setCallingConv(llvm::CallingConv::C);
        }
        
        load = this->getBuilder()->CreateLoad(objectValue, "chime_reference_create arg1");
        
        args.push_back(load);
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_reference_create return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(_functionChimeReferenceCreate, args.begin(), args.end(), "");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeReferenceGet(llvm::Value* referenceValue)
    {
        llvm::CallInst*           call;
        llvm::AllocaInst*         alloca;
        llvm::LoadInst*           load;
        std::vector<llvm::Value*> args;
        
        if (_functionChimeReferenceGet == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getChimeObjectPtrType());
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeReferenceGet = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_reference_get", this->getModule());
            _functionChimeReferenceGet->setCallingConv(llvm::CallingConv::C);
        }
        
        load = this->getBuilder()->CreateLoad(referenceValue, "chime_reference_get arg1:reference");
        
        args.push_back(load);
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_reference_get return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(_functionChimeReferenceGet, args.begin(), args.end(), "");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    void RuntimeInterface::callChimeReferenceSet(llvm::Value* referenceValue, llvm::Value* objectValue)
    {
        llvm::CallInst*           call;
        llvm::LoadInst*           referenceLoad;
        llvm::LoadInst*           objectLoad;
        std::vector<llvm::Value*> args;
        
        if (_functionChimeReferenceSet == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getChimeObjectPtrType());
            functionArgs.push_back(this->getChimeObjectPtrType());
            
            functionType = llvm::FunctionType::get(this->getVoidPtrType(), functionArgs, false);
            
            _functionChimeReferenceSet = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_reference_set", this->getModule());
            _functionChimeReferenceSet->setCallingConv(llvm::CallingConv::C);
        }
        
        referenceLoad = this->getBuilder()->CreateLoad(referenceValue, "chime_reference_set param1:reference");
        objectLoad    = this->getBuilder()->CreateLoad(objectValue, "chime_reference_set param2:object");
        
        args.push_back(referenceLoad);
        args.push_back(objectLoad);
        
        call = this->getBuilder()->CreateCall(_functionChimeReferenceSet, args.begin(), args.end(), "");
        call->setTailCall(false);
    }
    
    llvm::Value* RuntimeInterface::callChimeArrayCreateWithLength(llvm::Value* initialLength)
    {
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        if (_functionChimeArrayCreateWithLength == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(llvm::IntegerType::get(this->getContext(), 64));
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeArrayCreateWithLength = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_array_create_with_length", this->getModule());
            _functionChimeArrayCreateWithLength->setCallingConv(llvm::CallingConv::C);
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_array_create_with_length return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(_functionChimeArrayCreateWithLength, initialLength, "");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeArrayAppend(llvm::Value* arrayValue, llvm::Value* objectValue)
    {
        llvm::CallInst*           call;
        llvm::LoadInst*           arrayLoad;
        llvm::LoadInst*           objectLoad;
        llvm::AllocaInst*         alloca;
        std::vector<llvm::Value*> args;
        
        if (_functionChimeArrayAppend == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getChimeObjectPtrType());
            functionArgs.push_back(this->getChimeObjectPtrType());
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeArrayAppend = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_array_append", this->getModule());
            _functionChimeArrayAppend->setCallingConv(llvm::CallingConv::C);
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_array_append return");
        alloca->setAlignment(8);
        
        arrayLoad  = this->getBuilder()->CreateLoad(arrayValue,  "chime_array_append param1:array");
        objectLoad = this->getBuilder()->CreateLoad(objectValue, "chime_array_append param2:object");
        
        args.push_back(arrayLoad);
        args.push_back(objectLoad);
        
        call = this->getBuilder()->CreateCall(_functionChimeArrayAppend, args.begin(), args.end(), "");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeFloatCreate(llvm::Value* doubleValue)
    {
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        if (_functionChimeFloatCreate == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(llvm::Type::getDoubleTy(this->getContext()));
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeFloatCreate = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_float_create", this->getModule());
            _functionChimeFloatCreate->setCallingConv(llvm::CallingConv::C);
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_float_create return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(_functionChimeFloatCreate, doubleValue, "");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeHashCreate()
    {
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        if (_functionChimeHashCreate == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeHashCreate = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_hash_create", this->getModule());
            _functionChimeHashCreate->setCallingConv(llvm::CallingConv::C);
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_hash_create return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(_functionChimeHashCreate, "");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeHashSet(llvm::Value* hashValue, llvm::Value* keyValue, llvm::Value* valueValue)
    {
        llvm::CallInst*           call;
        llvm::LoadInst*           hashLoad;
        llvm::LoadInst*           keyLoad;
        llvm::LoadInst*           valueLoad;
        llvm::AllocaInst*         alloca;
        std::vector<llvm::Value*> args;
        
        if (_functionChimeHashSet == NULL)
        {
            std::vector<const llvm::Type*> functionArgs;
            llvm::FunctionType*            functionType;
            
            functionArgs.push_back(this->getChimeObjectPtrType());
            functionArgs.push_back(this->getChimeObjectPtrType());
            functionArgs.push_back(this->getChimeObjectPtrType());
            
            functionType = llvm::FunctionType::get(this->getChimeObjectPtrType(), functionArgs, false);
            
            _functionChimeHashSet = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "chime_hash_set", this->getModule());
            _functionChimeHashSet->setCallingConv(llvm::CallingConv::C);
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_hash_set return");
        alloca->setAlignment(8);
        
        hashLoad  = this->getBuilder()->CreateLoad(hashValue,  "chime_hash_set param1:hash");
        keyLoad   = this->getBuilder()->CreateLoad(keyValue,   "chime_hash_set param2:key");
        valueLoad = this->getBuilder()->CreateLoad(valueValue, "chime_hash_set param2:value");
        
        args.push_back(hashLoad);
        args.push_back(keyLoad);
        args.push_back(valueLoad);
        
        call = this->getBuilder()->CreateCall(_functionChimeHashSet, args.begin(), args.end(), "");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
}
