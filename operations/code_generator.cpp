#include "code_generator.h"
#include <assert.h>

namespace chime
{
    code_generator::code_generator()
    {
        _module       = NULL;
        _builder      = new llvm::IRBuilder<>(llvm::getGlobalContext());
        _scope_values = new std::map<std::string, llvm::Value*>();
    }
    
    code_generator::~code_generator()
    {
        delete _scope_values;
        delete _builder;
        
        if (_module)
            delete _module;
    }
    
    llvm::IRBuilder<>* code_generator::builder(void) const
    {
        return _builder;
    }
    
    llvm::Module* code_generator::module(void) const
    {
        return _module;
    }
    
    llvm::Function* code_generator::define_chime_runtime_initialize(void)
    {
        llvm::LLVMContext&             context = this->module()->getContext();
        std::vector<const llvm::Type*> function_args;
        llvm::FunctionType*            function_type;
        llvm::Function*                function_initialize_runtime;
        
        function_type = llvm::FunctionType::get(llvm::Type::getVoidTy(context), function_args, false);
        
        function_initialize_runtime = llvm::Function::Create(function_type, llvm::GlobalValue::ExternalLinkage, "chime_runtime_initialize", this->module());
        function_initialize_runtime->setCallingConv(llvm::CallingConv::C);
        
        return function_initialize_runtime;
    }
    
    void code_generator::make_main(void)
    {
        llvm::LLVMContext&             context = this->module()->getContext();
        const llvm::IntegerType*       int32_type;
        llvm::Type*                    int8_ptr_type;
        std::vector<const llvm::Type*> main_function_args;
        llvm::FunctionType*            main_function_type; 
        llvm::Function*                main_function;
        llvm::Function*                initialize_function;
        
        int32_type    = llvm::IntegerType::get(context, 32);
        int8_ptr_type = llvm::PointerType::get(llvm::IntegerType::get(context, 8), 0);
        
        main_function_args.push_back(int32_type);
        main_function_args.push_back(llvm::PointerType::get(int8_ptr_type, 0));
        
        main_function_type = llvm::FunctionType::get(int32_type, main_function_args, false);
          
        main_function = llvm::Function::Create(main_function_type, llvm::GlobalValue::ExternalLinkage, "main", this->module());
        main_function->setCallingConv(llvm::CallingConv::C);
        
        // function body
        llvm::BasicBlock* label_entry = llvm::BasicBlock::Create(context, "entry", main_function, 0);
        this->builder()->SetInsertPoint(label_entry);
        
        initialize_function = this->define_chime_runtime_initialize();
        
        this->builder()->CreateCall(initialize_function, "");
        
        //this->builder()->CreateRet(llvm::ConstantInt::get(context, llvm::APInt(32, 2, false)));
    }
    
    void code_generator::generate(ast::node* node, const char* module_name)
    {
        std::vector<ast::node*>::iterator i;
        
        assert(node != NULL);
        
        _module = new llvm::Module(module_name, llvm::getGlobalContext());
        
        this->make_main();
        
        // for (i=node->children()->begin(); i < node->children()->end(); i++)
        // {
        //     (*i)->codegen(*this);
        // }
        
        this->builder()->CreateRet(llvm::ConstantInt::get(this->module()->getContext(), llvm::APInt(32, 2, false)));
        
        llvm::verifyModule(*this->module(), llvm::PrintMessageAction);
    }
}
