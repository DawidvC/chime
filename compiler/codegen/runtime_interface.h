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
        llvm::Type*         getVoidPtrType(void);
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
        llvm::Value* callChimeRuntimeGetTrait(llvm::Value* traitNamePtr);
        
        llvm::Value* callChimeRuntimeLoad(llvm::Value* namePtr);
        
        llvm::Function* createModuleInitFunction(const std::string& name);
        llvm::Value*    callModuleInitFunction(const std::string& name);
        
        void            callChimeRuntimeSetInstanceMethod(llvm::Value* classValue, llvm::Value* functionNamePtr, llvm::Function* function);
        void            callChimeRuntimeSetClassMethod(llvm::Value* classValue, llvm::Value* functionNamePtr, llvm::Function* function);
        
        // class functions
        void         callChimeClassIncludeTrait(llvm::Value* classPtr, llvm::Value* traitPtr);
        
        // trait functions
        llvm::Value* callChimeTraitCreate(llvm::Value* traitNamePtr);
        
        // object functions
        llvm::Value* callChimeObjectCreate(llvm::Value* classPtr);
        llvm::Value* callChimeObjectGetAttribute(llvm::Value* objectValue, llvm::Value* attributeNamePtr);
        void         callChimeObjectSetAttribute(llvm::Value* objectValue, llvm::Value* attributeNamePtr, llvm::Value* attributeValue);
        
        llvm::Value* callChimeObjectInvoke(llvm::Value* objectValue, llvm::Value* namePtr, std::vector<llvm::Value*> args);
        
        void         callChimeObjectRetain(llvm::Value* objectValue);
        void         callChimeObjectRelease(llvm::Value* objectValue);
        
        // literal functions
        llvm::Value* callChimeLiteralEncodeInteger(llvm::Value* integerValue);
        llvm::Value* callChimeLiteralEncodeBoolean(llvm::Value* booleanValue);
        
        llvm::Value* callChimeStringCreateWithCString(llvm::Value* cStringPtr, llvm::Value* length);
        
        llvm::Value* callChimeRangeCreate(llvm::Value* startValue, llvm::Value* endValue);
        
        llvm::Constant* getChimeLiteralNull();
        llvm::Constant* getChimeLiteralTrue();
        llvm::Constant* getChimeLiteralFalse();
        
        // closure functions
        llvm::Value* callChimeClosureCreate(llvm::Function* function);
        
        // reference functions
        llvm::Value* callChimeReferenceCreate(llvm::Value* objectValue);
        llvm::Value* callChimeReferenceGet(llvm::Value* referenceValue);
        void         callChimeReferenceSet(llvm::Value* referenceValue, llvm::Value* objectValue);
        
        // array functions
        llvm::Value* callChimeArrayCreateWithLength(llvm::Value* initialLength);
        llvm::Value* callChimeArrayAppend(llvm::Value* arrayValue, llvm::Value* objectValue);
        
        // float functions
        llvm::Value* callChimeFloatCreate(llvm::Value* doubleValue);
        
        // hash functions
        llvm::Value* callChimeHashCreate();
        llvm::Value* callChimeHashSet(llvm::Value* hashValue, llvm::Value* keyValue, llvm::Value* valueValue);
        
    private:
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
        llvm::Function*     _functionChimeRuntimeGetTrait;
        llvm::Function*     _functionChimeRuntimeLoad;
        llvm::Function*     _functionChimeObjectCreate;
        llvm::Function*     _functionChimeRuntimeSetInstanceMethod;
        llvm::Function*     _functionChimeRuntimeSetClassMethod;
        llvm::Function*     _functionChimeClassIncludeTrait;
        llvm::Function*     _functionChimeTraitCreate;
        llvm::Function*     _functionChimeObjectGetAttribute;
        llvm::Function*     _functionChimeObjectSetAttribute;
        llvm::Function*     _functionChimeObjectInvoke0;
        llvm::Function*     _functionChimeObjectInvoke1;
        llvm::Function*     _functionChimeObjectInvoke2;
        llvm::Function*     _functionChimeObjectInvoke3;
        llvm::Function*     _functionChimeLiteralEncodeInteger;
        llvm::Function*     _functionChimeLiteralEncodeBoolean;
        llvm::Function*     _functionChimeStringCreateWithCString;
        llvm::Function*     _functionChimeRangeCreate;
        llvm::Function*     _functionChimeClosureCreate;
        llvm::Function*     _functionChimeReferenceCreate;
        llvm::Function*     _functionChimeReferenceGet;
        llvm::Function*     _functionChimeReferenceSet;
        llvm::Function*     _functionChimeArrayCreateWithLength;
        llvm::Function*     _functionChimeArrayAppend;
        llvm::Function*     _functionChimeFloatCreate;
        llvm::Function*     _functionChimeHashCreate;
        llvm::Function*     _functionChimeHashSet;
        
        std::map<std::string, llvm::Function*> _runtimeFunctions;
        
        llvm::Constant*     _literalNull;
    };
}

#endif // RUNTIME_INTERFACE_H
