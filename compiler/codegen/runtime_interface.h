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
        llvm::Type*         getChimeObjectPtrPtrType(void);
        llvm::Type*         getChimeClassPtrType(void);
        llvm::Type*         getChimeClosurePtrType(void);
        llvm::FunctionType* getChimeModuleInitFunctionType(void);
        
        // runtime functions
        void         callChimeRuntimeInitialize(void);
        void         callChimeLibraryInitialize(void);
        
        llvm::Value* callChimeRuntimeCreateClass(llvm::Value* classNamePtr, llvm::Value* superclassObjectPtr);
        llvm::Value* callChimeRuntimeGetClass(llvm::Value* classNamePtr);
        
        llvm::Value* callChimeRuntimeLoad(llvm::Value* namePtr);
        
        llvm::Function* createModuleInitFunction(const std::string& name);
        llvm::Value*    callModuleInitFunction(const std::string& name);
        
        void            callChimeRuntimeSetInstanceMethod(llvm::Value* classValue, llvm::Value* functionNamePtr, llvm::Function* function);
        
        // object functions
        llvm::Value* callChimeObjectCreate(llvm::Value* classPtr);
        llvm::Value* callChimeObjectGetAttribute(llvm::Value* objectValue, llvm::Value* attributeNamePtr);
        void         callChimeObjectSetAttribute(llvm::Value* objectValue, llvm::Value* attributeNamePtr, llvm::Value* attributeValue);
        
        llvm::Value* callChimeObjectInvoke(llvm::Value* objectValue, llvm::Value* namePtr, std::vector<llvm::Value*> args);
        
        // literal functions
        llvm::Value* callChimeLiteralEncodeInteger(llvm::Value* integerValue);
        llvm::Value* callChimeLiteralEncodeBoolean(llvm::Value* booleanValue);
        
        llvm::Value* callChimeStringCreateWithCString(llvm::Value* cStringPtr);
        
        llvm::Value* getChimeLiteralNull();
        
        // closure functions
        llvm::Value* callChimeClosureCreate(llvm::Function* function);
        llvm::Value* callChimeClosureGetEnvironment(llvm::Value* closureValue);
        void         callChimeClosureSetEnvironment(llvm::Value* closureValue, llvm::Value* environmentPtrValue);
        
    protected:
        llvm::Module*       _module;
        llvm::IRBuilder<>*  _builder;
        
        llvm::PointerType*  _objectPtrType;
        llvm::PointerType*  _objectPtrPtrType;
        llvm::PointerType*  _classPtrType;
        llvm::PointerType*  _closurePtrType;
        llvm::Type*         _cStringPtrType;
        llvm::FunctionType* _chimeFunctionType;
        
        llvm::Function*     _functionChimeRuntimeInitialize;
        llvm::Function*     _functionChimeLibraryInitialize;
        llvm::Function*     _functionChimeRuntimeCreateClass;
        llvm::Function*     _functionChimeRuntimeGetClass;
        llvm::Function*     _functionChimeRuntimeLoad;
        llvm::Function*     _functionChimeObjectCreate;
        llvm::Function*     _functionChimeRuntimeSetInstanceMethod;
        llvm::Function*     _functionChimeObjectGetAttribute;
        llvm::Function*     _functionChimeObjectSetAttribute;
        llvm::Function*     _functionChimeObjectInvoke0;
        llvm::Function*     _functionChimeObjectInvoke1;
        llvm::Function*     _functionChimeObjectInvoke2;
        llvm::Function*     _functionChimeObjectInvoke3;
        llvm::Function*     _functionChimeLiteralEncodeInteger;
        llvm::Function*     _functionChimeLiteralEncodeBoolean;
        llvm::Function*     _functionChimeStringCreateWithCString;
        llvm::Function*     _functionChimeClosureCreate;
        llvm::Function*     _functionChimeClosureGetEnvironment;
        llvm::Function*     _functionChimeClosureSetEnvironment;
        
        llvm::Value*        _literalNull;
    };
}

#endif // RUNTIME_INTERFACE_H
