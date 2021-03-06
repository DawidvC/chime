// code_generator.h
//

#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

namespace chime
{
    class Root;
    class ScopedNode;
}

#include "compiler/codegen/runtime_interface.h"

#include "llvm/DerivedTypes.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/IRBuilder.h"

#include <map>

namespace chime
{
    class code_generator
    {
    public:
        code_generator(void);
        virtual ~code_generator(void);
        
        llvm::IRBuilder<>*        builder(void) const;
        llvm::Module*             module(void) const;
        llvm::LLVMContext&        getContext(void) const;
        llvm::LLVMContext&        get_context(void) const;
        RuntimeInterface*         getRuntime(void) const;
        std::vector<std::string>* getImportedNamespaces(void) const;
        
        ScopedNode*       getCurrentScope() const;
        void              setCurrentScope(ScopedNode* node);
        void              pushScope(ScopedNode* scope);
        void              popScope();
        
        llvm::Value*      getConstantString(std::string str);
        llvm::Value*      make_constant_string(std::string str);
        
        llvm::Type*       get_c_string_ptr_type(void);
        
        llvm::AllocaInst* insertChimeObjectAlloca(void);
        llvm::AllocaInst* insert_chime_object_alloca(void);
        
        llvm::Value*      createCondition(llvm::Value* cond);
        llvm::Function*   createFunction(llvm::FunctionType* type, const std::string name);
        
        void generate(Root* node, const std::string& moduleName, bool asMain);
        
    private:
        void generateMainFunction();
        void generateModuleInitFunction(const std::string& moduleName);
        
    private:
        llvm::Module*             _module;
        llvm::IRBuilder<>*        _builder;
        std::vector<std::string>* _importedNamespaces;
        
        ScopedNode*               _currentScope;
        
        llvm::PointerType*        _object_ptr_type;
        llvm::Type*               _c_string_ptr_type;
        llvm::FunctionType*       _chime_function_type;
        
        RuntimeInterface*         _runtimeInterface;
    };
}

#endif // CODE_GENERATOR_H
