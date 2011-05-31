#include "codegen/runtime_interface.h"

namespace chime
{
    RuntimeInterface::RuntimeInterface(llvm::Module* module, llvm::IRBuilder<>* builder)
    {
        _module            = module;
        _builder           = builder;
        
        _objectPtrType     = NULL;
        _chimeFunctionType = NULL;
        
        _functionChimeRuntimeInitialize = NULL;
        _functionChimeLibraryInitialize = NULL;
    }
    
    RuntimeInterface::~RuntimeInterface()
    {
    }
    
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
    
    llvm::Type* RuntimeInterface::getChimeObjectPtrType(llvm::LLVMContext& context)
    {
        llvm::OpaqueType* objectStructType;
        
        if (_objectPtrType)
            return _objectPtrType;
            
        objectStructType = llvm::OpaqueType::get(context);
        
        this->getModule()->addTypeName("struct._chime_object", objectStructType);
         
        _objectPtrType = llvm::PointerType::get(objectStructType, 0);
        
        return _objectPtrType;
    }
    
    llvm::FunctionType* RuntimeInterface::getChimeFunctionType(llvm::LLVMContext& context)
    {
        std::vector<const llvm::Type*> functionArgs;
        
        if (_chimeFunctionType)
            return _chimeFunctionType;
        
        functionArgs.push_back(this->getChimeObjectPtrType(context));
        functionArgs.push_back(llvm::PointerType::get(llvm::IntegerType::get(context, 8), 0));
        
        _chimeFunctionType = llvm::FunctionType::get(this->getChimeObjectPtrType(context), functionArgs, true);
        
        return _chimeFunctionType;
    }
    
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
}
