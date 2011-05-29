#include "codegen/runtime_interface.h"

namespace chime
{
    RuntimeInterface::RuntimeInterface(llvm::Module* module)
    {
        _module            = module;
        
        _objectPtrType     = NULL;
        _chimeFunctionType = NULL;
    }
    
    RuntimeInterface::~RuntimeInterface()
    {
    }
    
    llvm::Module* RuntimeInterface::getModule(void) const
    {
        return _module;
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
}