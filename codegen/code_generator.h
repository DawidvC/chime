// code_generator.h
//

#ifndef CODE_GENERATOR
#define CODE_GENERATOR

#include <map>

#include "llvm/DerivedTypes.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/IRBuilder.h"

#include "ast/node.h"
#include "codegen/runtime_interface.h"

namespace chime
{
    class code_generator
    {
    public:
        code_generator(void);
        virtual ~code_generator(void);
        
        llvm::IRBuilder<>* builder(void) const;
        llvm::Module*      module(void) const;
        llvm::LLVMContext& get_context(void) const;
        RuntimeInterface*  getRuntime(void) const;
        
        llvm::Value*       make_constant_string(std::string str);
        
        void               set_value_for_identifier(std::string name, llvm::Value* value);
        llvm::Value*       value_for_identifier(std::string name);
        
        void               current_method_target(llvm::Value* target);
        llvm::Value*       current_method_target(void);
        
        llvm::Type*         get_c_string_ptr_type(void);
        
        llvm::AllocaInst*   insert_chime_object_alloca(void);
        
        llvm::Value*    call_chime_runtime_get_class(llvm::Value* class_name_ptr);
        llvm::Value*    call_chime_object_invoke(llvm::Value* object_value, std::string name, std::vector<llvm::Value*> args);
        llvm::Value*    call_chime_object_invoke_current_target(std::string name, std::vector<llvm::Value*> args);
        llvm::Value*    call_chime_literal_encode_integer(signed long value);
        llvm::Value*    call_chime_literal_encode_boolean(unsigned char value);
        llvm::Value*    get_chime_literal_null(void);
        llvm::Value*    call_chime_string_create_with_c_string(std::string str);
        void            call_chime_object_set_function(llvm::Value* object_value, std::string name, llvm::Function* function, unsigned int arity);
        
        void generate(ast::node* node, const char* module_name);
        
    protected:
        llvm::Module*                        _module;
        llvm::IRBuilder<>*                   _builder;
        std::map<std::string, llvm::Value*>* _scope_values;
        llvm::Value*                         _current_method_target;
        
        llvm::PointerType*                   _object_ptr_type;
        llvm::Type*                          _c_string_ptr_type;
        llvm::FunctionType*                  _chime_function_type;
        
        RuntimeInterface*                    _runtimeInterface;
        
        void make_main(void);
    };
}

#endif
