// runtime_interface.h
//

#ifndef RUNTIME_INTERFACE
#define RUNTIME_INTERFACE

#include "llvm/DerivedTypes.h"
#include "llvm/Module.h"
#include "llvm/Support/IRBuilder.h"

namespace chime
{
    class RuntimeInterface
    {
    public:
        RuntimeInterface(llvm::Module* module, llvm::IRBuilder<>* builder);
        virtual ~RuntimeInterface();
        
        // accessors
        llvm::Module*      getModule(void) const;
        llvm::LLVMContext& getContext(void) const;
        llvm::IRBuilder<>* getBuilder(void) const;
        
        // types
        llvm::Type*         getChimeObjectPtrType(llvm::LLVMContext& context);
        llvm::FunctionType* getChimeFunctionType(llvm::LLVMContext& context);
        
        // runtime functions
        void         callChimeRuntimeInitialize(void);
        void         callChimeLibraryInitialize(void);
        
        // llvm::Value* callChimeRuntimeGetClass(llvm::Value* class_name_ptr);
        // 
        // // object functions
        // void         callChimeObjectSetFunction(llvm::Value* object_value, std::string name, llvm::Function* function, unsigned int arity);
        // 
        // llvm::Value* callChimeObjectInvoke(llvm::Value* object_value, std::string name, std::vector<llvm::Value*> args);
        // 
        // // literal functions
        // 
        // llvm::Value* callChimeLiteralEncodeInteger(signed long value);
        // llvm::Value* callChimeLiteralEncodeBoolean(unsigned char value);
        // 
        // llvm::Value* callChimeStringCreateWithCString(std::string str);
        
    protected:
        llvm::Module*       _module;
        llvm::IRBuilder<>*  _builder;
        
        llvm::PointerType*  _objectPtrType;
        llvm::Type*         _cStringPtrType;
        llvm::FunctionType* _chimeFunctionType;
        
        llvm::Function*     _functionChimeRuntimeInitialize;
        llvm::Function*     _functionChimeLibraryInitialize;
    };
}

#endif
