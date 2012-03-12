#include "runtime_interface.h"
#include "Function.h"

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
        
        _literalNull = NULL;
    }
    
    RuntimeInterface::~RuntimeInterface()
    {
    }
    
// Accessors
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

// Basic Types
    llvm::Type* RuntimeInterface::getVoidPtrType(void)
    {
        return Function::getVoidPtrType(this->getModule());
    }
    
    llvm::Type* RuntimeInterface::getCStringPtrType(void)
    {
        return Function::getCStringPointerType(this->getModule());
    }

// Chime Types
    llvm::Type* RuntimeInterface::getChimeObjectPtrType(void)
    {
        llvm::StructType* objectStructType;
        
        if (_objectPtrType)
            return _objectPtrType;
        
        objectStructType = llvm::StructType::create(this->getContext(), "struct._chime_object");
        
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
        llvm::StructType* classStructType;
        
        if (_classPtrType)
            return _classPtrType;
            
        classStructType = llvm::StructType::create(this->getContext(), "struct._chime_class");
         
        _classPtrType = llvm::PointerType::get(classStructType, 0);
        
        return _classPtrType;
    }
    
    llvm::Type* RuntimeInterface::getChimeClosurePtrType(void)
    {
        llvm::StructType* closureStructType;
        //std::vector<llvm::Type*> bodyTypes;
        
        if (_closurePtrType)
            return _closurePtrType;
        
        closureStructType = llvm::StructType::create(this->getContext(), "struct._chime_closure");
        
        _closurePtrType = llvm::PointerType::get(closureStructType, 0);
        
        return _closurePtrType;
    }
    
    llvm::FunctionType* RuntimeInterface::getChimeModuleInitFunctionType()
    {
        return Function::voidReturnNoArgsFunction(this->getModule(), "module_init").type();
    }
    
// Runtime Functions
    void RuntimeInterface::callChimeRuntimeInitialize(void)
    {
        llvm::Function* function;
        
        function = _runtimeFunctions["chime_runtime_initialize"];
        if (function == NULL)
        {
            function = Function::voidReturnNoArgsFunction(this->getModule(), "chime_runtime_initialize").create();
            _runtimeFunctions["chime_runtime_initialize"] = function;
        }
        
        this->getBuilder()->CreateCall(function, "");
    }
    
    void RuntimeInterface::callChimeLibraryInitialize(void)
    {
        llvm::Function* function;
        
        function = _runtimeFunctions["chime_library_initialize"];
        if (function == NULL)
        {
            function = Function::voidReturnNoArgsFunction(this->getModule(), "chime_library_initialize").create();
            _runtimeFunctions["chime_library_initialize"] = function;
        }
        
        this->getBuilder()->CreateCall(function, "");
    }
    
    llvm::Value* RuntimeInterface::callChimeRuntimeCreateClass(llvm::Value* classNamePtr, llvm::Value* superclassObjectPtr)
    {
        llvm::Function*   function;
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        function = _runtimeFunctions["chime_runtime_create_class"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_runtime_create_class", this->getChimeObjectPtrType());
            
            f.addArgument(this->getCStringPtrType());
            f.addArgument(this->getChimeObjectPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_runtime_create_class"] = function;
        }
        
        llvm::LoadInst*           loadedObjectPtr;
        std::vector<llvm::Value*> args;
        
        loadedObjectPtr = this->getBuilder()->CreateLoad(superclassObjectPtr, "instance of superclass");
        
        args.push_back(classNamePtr);
        args.push_back(loadedObjectPtr);
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "superclass object pointer");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(function, llvm::ArrayRef<llvm::Value*>(args), "create class");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeRuntimeGetClass(llvm::Value* classNamePtr)
    {
        llvm::Function*   function;
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        function = _runtimeFunctions["chime_runtime_get_class"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_runtime_get_class", this->getChimeObjectPtrType());
            
            f.addArgument(this->getCStringPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_runtime_get_class"] = function;
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "class object pointer");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(function, classNamePtr, "chime_runtime_get_class: call");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeRuntimeGetTrait(llvm::Value* traitNamePtr)
    {
        llvm::Function*   function;
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        function = _runtimeFunctions["chime_runtime_get_trait"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_runtime_get_trait", this->getChimeObjectPtrType());
            
            f.addArgument(this->getCStringPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_runtime_get_trait"] = function;
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_runtime_get_trait: return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(function, traitNamePtr, "chime_runtime_get_trait call");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeRuntimeLoad(llvm::Value* namePtr)
    {
        llvm::Function*   function;
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        function = _runtimeFunctions["chime_runtime_load"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_runtime_load");
            
            f.addArgument(this->getCStringPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_runtime_load"] = function;
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_runtime_load: return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(function, namePtr, "chime_runtime_load call");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Function* RuntimeInterface::createModuleInitFunction(const std::string& name)
    {
        return Function::voidReturnNoArgsFunction(this->getModule(), "init_" + name).create();
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
    
    void RuntimeInterface::callChimeRuntimeSetInstanceMethod(llvm::Value* classValue, llvm::Value* functionNamePtr, llvm::Function* method)
    {
        llvm::Function*           function;
        llvm::CallInst*           call;
        llvm::LoadInst*           loadedObjectPtr;
        std::vector<llvm::Value*> args;
        llvm::Constant*           functionPtr;
        
        function = _runtimeFunctions["chime_runtime_set_instance_method"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_runtime_set_instance_method");
            
            f.addArgument(this->getChimeObjectPtrType());
            f.addArgument(Function::getCStringPointerType(this->getModule()));
            f.addArgument(Function::getVoidPtrType(this->getModule()));
            
            function = f.create();
            
            _runtimeFunctions["chime_runtime_set_instance_method"] = function;
        }
        
        loadedObjectPtr = this->getBuilder()->CreateLoad(classValue, "chime_runtime_set_instance_method: arg1");
        
        functionPtr = llvm::ConstantExpr::getCast(llvm::Instruction::BitCast, method, Function::getVoidPtrType(this->getModule()));
        
        args.push_back(loadedObjectPtr);
        args.push_back(functionNamePtr);
        args.push_back(functionPtr);
        
        call = this->getBuilder()->CreateCall(function, llvm::ArrayRef<llvm::Value*>(args), "");
        call->setTailCall(false);
    }
    
    void RuntimeInterface::callChimeRuntimeSetClassMethod(llvm::Value* classValue, llvm::Value* functionNamePtr, llvm::Function* method)
    {
        llvm::Function*           function;
        llvm::CallInst*           call;
        llvm::LoadInst*           loadedObjectPtr;
        std::vector<llvm::Value*> args;
        llvm::Constant*           functionPtr;
        
        function = _runtimeFunctions["chime_runtime_set_class_method"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_runtime_set_class_method");
            
            f.addArgument(this->getChimeObjectPtrType());
            f.addArgument(Function::getCStringPointerType(this->getModule()));
            f.addArgument(Function::getVoidPtrType(this->getModule()));
            
            function = f.create();
            
            _runtimeFunctions["chime_runtime_set_class_method"] = function;
        }
        
        loadedObjectPtr = this->getBuilder()->CreateLoad(classValue, "chime_runtime_set_class_method param1:class");
        
        functionPtr = llvm::ConstantExpr::getCast(llvm::Instruction::BitCast, method, this->getVoidPtrType());
        
        args.push_back(loadedObjectPtr);
        args.push_back(functionNamePtr);
        args.push_back(functionPtr);
        
        call = this->getBuilder()->CreateCall(function, llvm::ArrayRef<llvm::Value*>(args), "");
        call->setTailCall(false);
    }

// Class Functions
    void RuntimeInterface::callChimeClassIncludeTrait(llvm::Value* classPtr, llvm::Value* traitPtr)
    {
        llvm::Function*           function;
        llvm::CallInst*           call;
        llvm::LoadInst*           loadedClassPtr;
        llvm::LoadInst*           loadedTraitPtr;
        std::vector<llvm::Value*> args;
        
        function = _runtimeFunctions["chime_class_include_trait"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_class_include_trait");
            
            f.addArgument(this->getChimeObjectPtrType());
            f.addArgument(this->getChimeObjectPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_class_include_trait"] = function;
        }
        
        loadedClassPtr = this->getBuilder()->CreateLoad(classPtr, "chime_class_include_trait param1:class");
        loadedTraitPtr = this->getBuilder()->CreateLoad(traitPtr, "chime_class_include_trait param2:trait");
        
        args.push_back(loadedClassPtr);
        args.push_back(loadedTraitPtr);
        
        call = this->getBuilder()->CreateCall(function, llvm::ArrayRef<llvm::Value*>(args), "");
        call->setTailCall(false);
    }
    
// Trait Functions
    llvm::Value* RuntimeInterface::callChimeTraitCreate(llvm::Value* traitNamePtr)
    {
        llvm::Function*   function;
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        function = _runtimeFunctions["chime_trait_create"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_trait_create", this->getChimeObjectPtrType());
            
            f.addArgument(this->getCStringPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_trait_create"] = function;
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_trait_create: return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(function, traitNamePtr, "chime_trait_create call");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
// Object Functions
    llvm::Value* RuntimeInterface::callChimeObjectCreate(llvm::Value* classPtr)
    {
        llvm::Function*   function;
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        llvm::LoadInst*   loadedObjectPtr;
        
        function = _runtimeFunctions["chime_object_create"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_object_create");
            
            f.addArgument(this->getChimeObjectPtrType());
            f.setReturnType(this->getChimeObjectPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_object_create"] = function;
        }
        
        loadedObjectPtr = this->getBuilder()->CreateLoad(classPtr, "chime_object_create arg1");
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "instance pointer");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(function, loadedObjectPtr, "chime_object_create call");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeObjectGetAttribute(llvm::Value* objectValue, llvm::Value* attributeNamePtr)
    {
        llvm::Function*   function;
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        function = _runtimeFunctions["chime_object_get_attribute"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_object_get_attribute", this->getChimeObjectPtrType());
            
            f.addArgument(this->getChimeObjectPtrType());
            f.addArgument(this->getCStringPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_object_get_attribute"] = function;
        }
        
        llvm::LoadInst*           loadedObjectPtr;
        std::vector<llvm::Value*> args;
        
        loadedObjectPtr = this->getBuilder()->CreateLoad(objectValue, "chime_object_get_attribute arg1");
        
        args.push_back(loadedObjectPtr);
        args.push_back(attributeNamePtr);
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "object get attribute return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(function, llvm::ValueArrayRef(args), "chime_object_get_attribute call");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeObjectGetAttributeUnretained(llvm::Value* objectValue, llvm::Value* attributeNamePtr)
    {
        llvm::Function*   function;
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        function = _runtimeFunctions["chime_object_get_attribute_unretained"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_object_get_attribute_unretained");
            
            f.addArgument(this->getChimeObjectPtrType());
            f.addArgument(this->getCStringPtrType());
            f.setReturnType(this->getChimeObjectPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_object_get_attribute_unretained"] = function;
        }
        
        llvm::LoadInst*           loadedObjectPtr;
        std::vector<llvm::Value*> args;
        
        loadedObjectPtr = this->getBuilder()->CreateLoad(objectValue, "chime_object_get_attribute_unretained arg1:object");
        
        args.push_back(loadedObjectPtr);
        args.push_back(attributeNamePtr);
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_object_get_attribute_unretained return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(function, llvm::ArrayRef<llvm::Value*>(args), "");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    void RuntimeInterface::callChimeObjectSetAttribute(llvm::Value* objectValue, llvm::Value* attributeNamePtr, llvm::Value* attributeValue)
    {
        llvm::Function* function;
        llvm::CallInst* call;
        
        function = _runtimeFunctions["chime_object_set_attribute"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_object_set_attribute");
            
            f.addArgument(this->getChimeObjectPtrType());
            f.addArgument(this->getCStringPtrType());
            f.addArgument(this->getChimeObjectPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_object_set_attribute"] = function;
        }
        
        llvm::LoadInst*           loadedObjectPtr;
        llvm::LoadInst*           loadedValuePtr;
        std::vector<llvm::Value*> args;
        
        loadedObjectPtr = this->getBuilder()->CreateLoad(objectValue, "chime_object_set_attribute arg2:instance");
        loadedValuePtr  = this->getBuilder()->CreateLoad(attributeValue, "chime_object_set_attribute arg2:value");
        
        args.push_back(loadedObjectPtr);
        args.push_back(attributeNamePtr);
        args.push_back(loadedValuePtr);
        
        call = this->getBuilder()->CreateCall(function, llvm::ArrayRef<llvm::Value*>(args), "");
        call->setTailCall(false);
    }
    
    llvm::Value* RuntimeInterface::callChimeObjectInvoke(llvm::Value* objectValue, llvm::Value* namePtr, std::vector<llvm::Value*> args)
    {
        llvm::Function*                     function;
        llvm::CallInst*                     call;
        std::vector<llvm::Value*>::iterator it;
        llvm::AllocaInst*                   alloca;
        
        function = _runtimeFunctions["chime_object_invoke_0"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_object_invoke_0");
            
            f.setReturnType(this->getChimeObjectPtrType());
            
            f.addArgument(this->getChimeObjectPtrType());
            f.addArgument(this->getCStringPtrType());
            
            _runtimeFunctions["chime_object_invoke_0"] = f.create();
            
            f.setName("chime_object_invoke_1");
            f.addArgument(this->getChimeObjectPtrType());
            _runtimeFunctions["chime_object_invoke_1"] = f.create();
            
            f.setName("chime_object_invoke_2");
            f.addArgument(this->getChimeObjectPtrType());
            _runtimeFunctions["chime_object_invoke_2"] = f.create();
            
            f.setName("chime_object_invoke_3");
            f.addArgument(this->getChimeObjectPtrType());
            _runtimeFunctions["chime_object_invoke_3"] = f.create();
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
            case 0: function = _runtimeFunctions["chime_object_invoke_0"]; break;
            case 1: function = _runtimeFunctions["chime_object_invoke_1"]; break;
            case 2: function = _runtimeFunctions["chime_object_invoke_2"]; break;
            case 3: function = _runtimeFunctions["chime_object_invoke_3"]; break;
            default:
                assert(0 && "More than three arguments unsupported right now!");
                break;
        }
        
        call = this->getBuilder()->CreateCall(function, llvm::ArrayRef<llvm::Value*>(args), "object invoke");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    void RuntimeInterface::callChimeObjectRetain(llvm::Value* objectValue)
    {
        llvm::Function* function;
        llvm::CallInst* call;
        llvm::LoadInst* loadedObjectPtr;
        
        function = _runtimeFunctions["chime_object_retain"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_object_retain");
            
            f.addArgument(this->getChimeObjectPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_object_retain"] = function;
        }
        
        loadedObjectPtr = this->getBuilder()->CreateLoad(objectValue, "chime_object_retain arg1:instance");
        
        call = this->getBuilder()->CreateCall(function, loadedObjectPtr, "");
        call->setTailCall(false);
    }
    
    void RuntimeInterface::callChimeObjectRelease(llvm::Value* objectValue)
    {
        llvm::Function* function;
        llvm::CallInst* call;
        llvm::LoadInst* loadedObjectPtr;
        
        function = _runtimeFunctions["chime_object_release"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_object_release");
            
            f.addArgument(this->getChimeObjectPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_object_release"] = function;
        }
        
        loadedObjectPtr = this->getBuilder()->CreateLoad(objectValue, "chime_object_release arg1:instance");
        
        call = this->getBuilder()->CreateCall(function, loadedObjectPtr, "");
        call->setTailCall(false);
    }
    
// Literal Functions
    llvm::Value* RuntimeInterface::callChimeIntegerEncode(llvm::Value* integerValue)
    {
        llvm::Function*   function;
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        function = _runtimeFunctions["chime_integer_encode"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_integer_encode");
            
            f.addArgument(llvm::IntegerType::get(this->getContext(), 64));
            
            f.setReturnType(this->getChimeObjectPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_integer_encode"] = function;
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_integer_encode return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(function, integerValue, "chime_integer_encode call");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeBooleanEncode(llvm::Value* booleanValue)
    {
        llvm::Function*   function;
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        function = _runtimeFunctions["chime_boolean_encode"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_boolean_encode");
            
            f.addArgument(llvm::IntegerType::get(this->getContext(), 8));
            f.setReturnType(this->getChimeObjectPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_boolean_encode"] = function;
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "encode boolean return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(function, booleanValue, "chime_boolean_encode call");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeStringCreateWithCString(llvm::Value* cStringPtr, llvm::Value* lengthValue)
    {
        llvm::Function*           function;
        llvm::CallInst*           call;
        llvm::AllocaInst*         alloca;
        std::vector<llvm::Value*> args;
        
        function = _runtimeFunctions["chime_string_create_with_c_string"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_string_create_with_c_string");
            
            f.addArgument(this->getCStringPtrType());
            f.addArgument(llvm::IntegerType::get(this->getContext(), 64));
            f.setReturnType(this->getChimeObjectPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_string_create_with_c_string"] = function;
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "create string with c string");
        alloca->setAlignment(8);
        
        args.push_back(cStringPtr);
        args.push_back(lengthValue);
        
        call = this->getBuilder()->CreateCall(function, llvm::ValueArrayRef(args), "chime_string_create_with_c_string call");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeRangeCreate(llvm::Value* startValue, llvm::Value* endValue)
    {
        llvm::Function*           function;
        llvm::CallInst*           call;
        llvm::AllocaInst*         alloca;
        std::vector<llvm::Value*> args;
        
        function = _runtimeFunctions["chime_range_create"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_range_create");
            
            f.addArgument(this->getChimeObjectPtrType());
            f.addArgument(this->getChimeObjectPtrType());
            f.setReturnType(this->getChimeObjectPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_range_create"] = function;
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_range_create: return");
        alloca->setAlignment(8);
        
        startValue = this->getBuilder()->CreateLoad(startValue, "chime_range_create: arg1");
        endValue   = this->getBuilder()->CreateLoad(endValue,   "chime_range_create: arg2");
        
        args.push_back(startValue);
        args.push_back(endValue);
        
        call = this->getBuilder()->CreateCall(function, llvm::ValueArrayRef(args), "chime_range_create: call");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Constant* RuntimeInterface::getChimeLiteralNull()
    {
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
    
// Closure Functions
    llvm::Value* RuntimeInterface::callChimeClosureCreate(llvm::Function* closureFunction)
    {
        llvm::Function*   function;
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        llvm::Type*       voidPtrType;
        
        // a generic pointer (llvm does not support the void* type)
        voidPtrType = Function::getVoidPtrType(this->getModule());
        
        function = _runtimeFunctions["chime_closure_create"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_closure_create");
            
            f.addArgument(voidPtrType);
            f.setReturnType(this->getChimeObjectPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_closure_create"] = function;
        }
        
        llvm::Constant* functionPtr;
        
        functionPtr = llvm::ConstantExpr::getCast(llvm::Instruction::BitCast, closureFunction, voidPtrType);
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "new closure");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(function, functionPtr, "chime_closure_create: call");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
// Reference Functions
    llvm::Value* RuntimeInterface::callChimeReferenceCreate(llvm::Value* objectValue)
    {
        llvm::Function*   function;
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        llvm::LoadInst*   load;
        
        assert(objectValue);
        
        function = _runtimeFunctions["chime_reference_create"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_reference_create");
            
            f.addArgument(this->getChimeObjectPtrType());
            f.setReturnType(this->getChimeObjectPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_reference_create"] = function;
        }
        
        load = this->getBuilder()->CreateLoad(objectValue, "chime_reference_create arg1");
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_reference_create return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(function, load, "chime_reference_create: call");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeReferenceGet(llvm::Value* referenceValue)
    {
        llvm::Function*   function;
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        llvm::LoadInst*   load;
        
        function = _runtimeFunctions["chime_reference_get"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_reference_get");
            
            f.addArgument(this->getChimeObjectPtrType());
            f.setReturnType(this->getChimeObjectPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_reference_get"] = function;
        }
        
        load = this->getBuilder()->CreateLoad(referenceValue, "chime_reference_get arg1:reference");
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_reference_get return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(function, load, "chime_reference_get: call");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    void RuntimeInterface::callChimeReferenceSet(llvm::Value* referenceValue, llvm::Value* objectValue)
    {
        llvm::Function*           function;
        llvm::LoadInst*           referenceLoad;
        llvm::LoadInst*           objectLoad;
        llvm::CallInst*           call;
        std::vector<llvm::Value*> args;
        
        function = _runtimeFunctions["chime_reference_set"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_reference_set");
            
            f.addArgument(this->getChimeObjectPtrType());
            f.addArgument(this->getChimeObjectPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_reference_set"] = function;
        }
        
        referenceLoad = this->getBuilder()->CreateLoad(referenceValue, "chime_reference_set param1:reference");
        objectLoad    = this->getBuilder()->CreateLoad(objectValue, "chime_reference_set param2:object");
        
        args.push_back(referenceLoad);
        args.push_back(objectLoad);
        
        call = this->getBuilder()->CreateCall(function, llvm::ValueArrayRef(args), "");
        call->setTailCall(false);
    }
    
    llvm::Value* RuntimeInterface::callChimeArrayCreateWithLength(llvm::Value* initialLength)
    {
        llvm::Function*   function;
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        function = _runtimeFunctions["chime_array_create_with_length"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_array_create_with_length");
            
            f.addArgument(llvm::IntegerType::get(this->getContext(), 64));
            f.setReturnType(this->getChimeObjectPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_array_create_with_length"] = function;
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_array_create_with_length return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(function, initialLength, "chime_array_create_with_length: call");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeArrayAppend(llvm::Value* arrayValue, llvm::Value* objectValue)
    {
        llvm::Function*           function;
        llvm::CallInst*           call;
        llvm::LoadInst*           arrayLoad;
        llvm::LoadInst*           objectLoad;
        llvm::AllocaInst*         alloca;
        std::vector<llvm::Value*> args;
        
        function = _runtimeFunctions["chime_array_append"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_array_append");
            
            f.addArgument(this->getChimeObjectPtrType());
            f.addArgument(this->getChimeObjectPtrType());
            f.setReturnType(this->getChimeObjectPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_array_append"] = function;
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_array_append return");
        alloca->setAlignment(8);
        
        arrayLoad  = this->getBuilder()->CreateLoad(arrayValue,  "chime_array_append param1:array");
        objectLoad = this->getBuilder()->CreateLoad(objectValue, "chime_array_append param2:object");
        
        args.push_back(arrayLoad);
        args.push_back(objectLoad);
        
        call = this->getBuilder()->CreateCall(function, llvm::ValueArrayRef(args), "chime_array_append: call");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeFloatCreate(llvm::Value* doubleValue)
    {
        llvm::Function*   function;
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        function = _runtimeFunctions["chime_float_create"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_float_create");
            
            f.addArgument(llvm::Type::getDoubleTy(this->getContext()));
            f.setReturnType(this->getChimeObjectPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_float_create"] = function;
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_float_create return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(function, doubleValue, "chime_float_create: call");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeHashCreate()
    {
        llvm::Function*   function;
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        function = _runtimeFunctions["chime_hash_create"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_hash_create");
            
            f.setReturnType(this->getChimeObjectPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_hash_create"] = function;
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_hash_create return");
        alloca->setAlignment(8);
        
        call = this->getBuilder()->CreateCall(function, "chime_hash_create: call");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* RuntimeInterface::callChimeHashSet(llvm::Value* hashValue, llvm::Value* keyValue, llvm::Value* valueValue)
    {
        llvm::Function*           function;
        llvm::CallInst*           call;
        llvm::LoadInst*           hashLoad;
        llvm::LoadInst*           keyLoad;
        llvm::LoadInst*           valueLoad;
        llvm::AllocaInst*         alloca;
        std::vector<llvm::Value*> args;
        
        function = _runtimeFunctions["chime_hash_set"];
        if (function == NULL)
        {
            Function f(this->getModule(), "chime_hash_set");
            
            f.addArgument(this->getChimeObjectPtrType());
            f.addArgument(this->getChimeObjectPtrType());
            f.addArgument(this->getChimeObjectPtrType());
            f.setReturnType(this->getChimeObjectPtrType());
            
            function = f.create();
            
            _runtimeFunctions["chime_hash_set"] = function;
        }
        
        alloca = this->getBuilder()->CreateAlloca(this->getChimeObjectPtrType(), 0, "chime_hash_set return");
        alloca->setAlignment(8);
        
        hashLoad  = this->getBuilder()->CreateLoad(hashValue,  "chime_hash_set param1:hash");
        keyLoad   = this->getBuilder()->CreateLoad(keyValue,   "chime_hash_set param2:key");
        valueLoad = this->getBuilder()->CreateLoad(valueValue, "chime_hash_set param2:value");
        
        args.push_back(hashLoad);
        args.push_back(keyLoad);
        args.push_back(valueLoad);
        
        call = this->getBuilder()->CreateCall(function, llvm::ValueArrayRef(args), "");
        call->setTailCall(false);
        
        this->getBuilder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
}
