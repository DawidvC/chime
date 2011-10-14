// code_generator.h
//

#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

namespace ast
{
    class Root;
    class ScopedNode;
}

#include "compiler/codegen/runtime_interface.h"
#include "compiler/codegen/scopes/implementation_scope.h"

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
        
        ast::ScopedNode*       getCurrentScope() const;
        void                   setCurrentScope(ast::ScopedNode* node);
        void                   pushScope(ast::ScopedNode* scope);
        void                   popScope();
        ImplementationScopeRef getImplementationScope(void) const;
        void                   setImplementationScope(ImplementationScopeRef scope);
        std::vector<llvm::Function*>* getInitFunctions(void) const;
        
        llvm::Value*       getConstantString(std::string str);
        llvm::Value*       make_constant_string(std::string str);
        
        llvm::Type*        get_c_string_ptr_type(void);
        
        llvm::AllocaInst*  insertChimeObjectAlloca(void);
        llvm::AllocaInst*  insert_chime_object_alloca(void);
        
        llvm::Value*       createCondition(llvm::Value* cond);
        llvm::Function*    createFunction(const llvm::FunctionType* type, const std::string name);
        
        llvm::Value*    callModuleInitFunction(const std::string& name);
        
        void generate(ast::Root* node, const std::string& moduleName, bool asMain);
        
    protected:
        llvm::Module*                        _module;
        llvm::IRBuilder<>*                   _builder;
        std::vector<llvm::Function*>*        _initFunctions;
        llvm::Function*                      _internalInitFunction;
        ImplementationScopeRef               _implementationScope;
        std::vector<std::string>*            _importedNamespaces;
        
        ast::ScopedNode*                     _currentScope;
        
        llvm::PointerType*                   _object_ptr_type;
        llvm::Type*                          _c_string_ptr_type;
        llvm::FunctionType*                  _chime_function_type;
        
        RuntimeInterface*                    _runtimeInterface;
        
        void generateMainFunction(void);
        void fillInInternalInitFunction(void);
    };
    
    typedef code_generator CodeGenContext;
}

#endif // CODE_GENERATOR_H
