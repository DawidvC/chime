// runtime_interface.h
//

#ifndef RUNTIME_INTERFACE_H
#define RUNTIME_INTERFACE_H

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
        llvm::FunctionType* getChimeModuleInitFunctionType(void);
        
        // runtime functions
        void         callChimeRuntimeInitialize(void);
        void         callChimeLibraryInitialize(void);
        
        llvm::Value* callChimeRuntimeCreateClass(llvm::Value* classNamePtr, llvm::Value* superclassObjectPtr);
        llvm::Value* callChimeRuntimeGetClass(llvm::Value* classNamePtr);
        
        llvm::Value* callChimeRuntimeLoad(llvm::Value* namePtr);
        
        llvm::Function* createModuleInitFunction(const std::string& name);
        llvm::Value*    callModuleInitFunction(const std::string& name);
        
        // object functions
        llvm::Value* callChimeObjectCreate(llvm::Value* classPtr);
        void         callChimeObjectSetFunction(llvm::Value* objectValue, llvm::Value* propertyNamePtr, llvm::Function* function, unsigned int arity);
        llvm::Value* callChimeObjectGetAttribute(llvm::Value* objectValue, llvm::Value* attributeNamePtr);
        void         callChimeObjectSetAttribute(llvm::Value* objectValue, llvm::Value* attributeNamePtr, llvm::Value* attributeValue);
        
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
        llvm::Function*     _functionChimeRuntimeCreateClass;
        llvm::Function*     _functionChimeRuntimeGetClass;
        llvm::Function*     _functionChimeRuntimeLoad;
        llvm::Function*     _functionChimeObjectCreate;
        llvm::Function*     _functionChimeObjectSetFunction;
        llvm::Function*     _functionChimeObjectGetAttribute;
        llvm::Function*     _functionChimeObjectSetAttribute;
        llvm::Function*     _functionChimeObjectInvoke0;
        llvm::Function*     _functionChimeObjectInvoke1;
        llvm::Function*     _functionChimeObjectInvoke2;
        llvm::Function*     _functionChimeObjectInvoke3;
        llvm::Function*     _functionChimeLiteralEncodeInteger;
        llvm::Function*     _functionChimeLiteralEncodeBoolean;
        llvm::Function*     _functionChimeStringCreateWithCString;
        
        llvm::Value*        _literalNull;
    };
}

#endif // RUNTIME_INTERFACE_H
