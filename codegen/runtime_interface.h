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
        
        // basic types
        llvm::Type*         getCStringPtrType(void);
        
        // types
        llvm::Type*         getChimeObjectPtrType(void);
        llvm::FunctionType* getChimeFunctionType(void);
        
        // runtime functions
        void         callChimeRuntimeInitialize(void);
        void         callChimeLibraryInitialize(void);
        
        llvm::Value* callChimeRuntimeGetClass(llvm::Value* classNamePtr);
        
        // object functions
        void         callChimeObjectSetFunction(llvm::Value* object_value, llvm::Value* propertyNamePtr, llvm::Function* function, unsigned int arity);
        
        llvm::Value* callChimeObjectInvoke(llvm::Value* objectValue, llvm::Value* namePtr, std::vector<llvm::Value*> args);
        
        // literal functions
        llvm::Value* callChimeLiteralEncodeInteger(signed long value);
        llvm::Value* callChimeLiteralEncodeBoolean(unsigned char value);
        
        llvm::Value* callChimeStringCreateWithCString(llvm::Value* cStringPtr);
        
        llvm::Value* getChimeLiteralNull();
        
    protected:
        llvm::Module*       _module;
        llvm::IRBuilder<>*  _builder;
        
        llvm::PointerType*  _objectPtrType;
        llvm::Type*         _cStringPtrType;
        llvm::FunctionType* _chimeFunctionType;
        
        llvm::Function*     _functionChimeRuntimeInitialize;
        llvm::Function*     _functionChimeLibraryInitialize;
        llvm::Function*     _functionChimeRuntimeGetClass;
        llvm::Function*     _functionChimeObjectSetFunction;
        llvm::Function*     _functionChimeObjectInvoke;
        llvm::Function*     _functionChimeLiteralEncodeInteger;
        llvm::Function*     _functionChimeLiteralEncodeBoolean;
        llvm::Function*     _functionChimeStringCreateWithCString;
        
        llvm::Value*        _literalNull;
    };
}

#endif
