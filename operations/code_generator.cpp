#include "code_generator.h"
#include <assert.h>

namespace chime
{
    code_generator::code_generator()
    {
        _module       = NULL;
        _builder      = new llvm::IRBuilder<>(llvm::getGlobalContext());
        _scope_values = new std::map<std::string, llvm::Value*>();
        
        _object_ptr_type = NULL;
        _c_string_ptr_type = NULL;
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
    
    llvm::LLVMContext* code_generator::get_context(void) const
    {
        return &(this->module()->getContext());
    }
    
    llvm::Value* code_generator::make_constant_string(std::string str)
    {
        llvm::ArrayType*             string_array_type;
        llvm::GlobalVariable*        global_string;
        llvm::ConstantInt*           const_int32;
        std::vector<llvm::Constant*> const_ptr_indices;
        llvm::Constant*              const_ptr;
        llvm::Constant*              const_c_string_array;
        
        string_array_type = llvm::ArrayType::get(llvm::IntegerType::get(*this->get_context(), 8), str.length()+1);
        
        global_string = new llvm::GlobalVariable(*(this->module()), string_array_type, true, llvm::GlobalValue::PrivateLinkage, 0, ".str");
        
        const_int32 = llvm::ConstantInt::get(*this->get_context(), llvm::APInt(32, llvm::StringRef("0"), 10));
        
        const_ptr_indices.push_back(const_int32);
        const_ptr_indices.push_back(const_int32);
        
        const_ptr = llvm::ConstantExpr::getGetElementPtr(global_string, &const_ptr_indices[0], const_ptr_indices.size());
        
        const_c_string_array = llvm::ConstantArray::get(*this->get_context(), str.c_str(), true);
        
        global_string->setInitializer(const_c_string_array);
        
        return const_ptr;
    }
    
    void code_generator::set_value_for_identifier(std::string name, llvm::Value* value)
    {
        (*_scope_values)[name] = value;
    }
    
    llvm::Value* code_generator::value_for_identifier(std::string name)
    {
        return (*_scope_values)[name];
    }
    
    llvm::Type* code_generator::get_c_string_ptr_type(void)
    {
        if (!_c_string_ptr_type)
            _c_string_ptr_type = llvm::PointerType::get(llvm::IntegerType::get(*this->get_context(), 8), 0);
        
        return _c_string_ptr_type;
    }
    
    llvm::Type* code_generator::get_chime_object_ptr_type(void)
    {
        llvm::OpaqueType*  object_struct_type;
        
        if (_object_ptr_type)
            return _object_ptr_type;
            
        object_struct_type = llvm::OpaqueType::get(*this->get_context());
        
        this->module()->addTypeName("struct._chime_object", object_struct_type);
         
        _object_ptr_type = llvm::PointerType::get(object_struct_type, 0);
        
        return _object_ptr_type;
    }
    
    llvm::AllocaInst* code_generator::insert_chime_object_alloca(void)
    {
        llvm::AllocaInst* alloca;
        
        alloca = this->builder()->CreateAlloca(this->get_chime_object_ptr_type(), 0, "object pointer");
        alloca->setAlignment(8);
        
        return alloca;
    }
    
    void code_generator::call_chime_runtime_initialize(void)
    {
        llvm::Function* function_chime_runtime_initialize;
        
        function_chime_runtime_initialize = (llvm::Function*)this->value_for_identifier("chime_runtime_initialize");
        if (function_chime_runtime_initialize == NULL)
        {
            std::vector<const llvm::Type*> function_args;
            llvm::FunctionType*            function_type;
            
            function_type = llvm::FunctionType::get(llvm::Type::getVoidTy(*this->get_context()), function_args, false);
            
            function_chime_runtime_initialize = llvm::Function::Create(function_type, llvm::GlobalValue::ExternalLinkage, "chime_runtime_initialize", this->module());
            function_chime_runtime_initialize->setCallingConv(llvm::CallingConv::C);
        }
        
        this->builder()->CreateCall(function_chime_runtime_initialize, "");
    }
    
    llvm::Value* code_generator::call_chime_runtime_get_class(llvm::Value* class_name_ptr)
    {
        llvm::Function*   function_chime_runtime_get_class;
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        function_chime_runtime_get_class = (llvm::Function*)this->value_for_identifier("chime_runtime_get_class");
        if (function_chime_runtime_get_class == NULL)
        {
            std::vector<const llvm::Type*> function_args;
            llvm::FunctionType*            function_type;
            
            function_args.push_back(this->get_c_string_ptr_type());
            
            function_type = llvm::FunctionType::get(this->get_chime_object_ptr_type(), function_args, false);
            
            function_chime_runtime_get_class = llvm::Function::Create(function_type, llvm::GlobalValue::ExternalLinkage, "chime_runtime_get_class", this->module());
            function_chime_runtime_get_class->setCallingConv(llvm::CallingConv::C);
            
            this->set_value_for_identifier("chime_runtime_get_class", function_chime_runtime_get_class);
        }
        
        alloca = this->insert_chime_object_alloca();
        
        call = this->builder()->CreateCall(function_chime_runtime_get_class, class_name_ptr, "class lookup");
        call->setTailCall(false);
        
        this->builder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* code_generator::call_chime_object_invoke(llvm::Value* object_value, std::string name, std::vector<llvm::Value*> args)
    {
        llvm::Function*                     function_chime_object_invoke;
        llvm::CallInst*                     call;
        std::vector<llvm::Value*>::iterator it;
        llvm::AllocaInst*                   alloca;
        
        function_chime_object_invoke = (llvm::Function*)this->value_for_identifier("chime_object_invoke");
        if (function_chime_object_invoke == NULL)
        {
            std::vector<const llvm::Type*> function_args;
            llvm::FunctionType*            function_type;
            
            function_args.push_back(this->get_chime_object_ptr_type());
            function_args.push_back(this->get_c_string_ptr_type());
            
            function_type = llvm::FunctionType::get(this->get_chime_object_ptr_type(), function_args, true);
            
            function_chime_object_invoke = llvm::Function::Create(function_type, llvm::GlobalValue::ExternalLinkage, "chime_object_invoke", this->module());
            function_chime_object_invoke->setCallingConv(llvm::CallingConv::C);
            
            this->set_value_for_identifier("chime_object_invoke", function_chime_object_invoke);
        }
        
        llvm::Value*    property_name_ptr;
        llvm::LoadInst* loaded_object_ptr;
        
        property_name_ptr = this->make_constant_string(name);
        
        loaded_object_ptr = this->builder()->CreateLoad(object_value, "instance for object invoke");
        
        // grab an iterator and do some insertions such that
        // the object comes first, followed by the property, followed
        // by whatever arguments were supplied
        it = args.begin();
        it = args.insert(it, property_name_ptr);
        args.insert(it, loaded_object_ptr);
        
        alloca = this->insert_chime_object_alloca();
        
        call = this->builder()->CreateCall(function_chime_object_invoke, args.begin(), args.end(), "object invoke");
        call->setTailCall(false);
        
        this->builder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    llvm::Value* code_generator::call_chime_literal_encode_integer(signed long value)
    {
        llvm::Function*   function_chime_literal_encode_integer;
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        function_chime_literal_encode_integer = (llvm::Function*)this->value_for_identifier("chime_literal_encode_integer");
        if (function_chime_literal_encode_integer == NULL)
        {
            std::vector<const llvm::Type*> function_args;
            llvm::FunctionType*            function_type;
            
            function_args.push_back(llvm::IntegerType::get(*this->get_context(), 64));
            
            function_type = llvm::FunctionType::get(this->get_chime_object_ptr_type(), function_args, false);
            
            function_chime_literal_encode_integer = llvm::Function::Create(function_type, llvm::GlobalValue::ExternalLinkage, "chime_literal_encode_integer", this->module());
            function_chime_literal_encode_integer->setCallingConv(llvm::CallingConv::C);
            
            this->set_value_for_identifier("chime_literal_encode_integer", function_chime_literal_encode_integer);
        }
        
        llvm::Value* integer_value;
        
        alloca = this->insert_chime_object_alloca();
        
        integer_value = llvm::ConstantInt::get(*this->get_context(), llvm::APInt(64, value, 10));
        
        call = this->builder()->CreateCall(function_chime_literal_encode_integer, integer_value, "encode integer");
        call->setTailCall(false);
        
        this->builder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    llvm::Value* code_generator::call_chime_string_create_with_c_string(std::string str)
    {
        llvm::Function*   function_chime_string_create_with_c_string;
        llvm::CallInst*   call;
        llvm::AllocaInst* alloca;
        
        function_chime_string_create_with_c_string = (llvm::Function*)this->value_for_identifier("chime_string_create_with_c_string");
        if (function_chime_string_create_with_c_string == NULL)
        {
            std::vector<const llvm::Type*> function_args;
            llvm::FunctionType*            function_type;
            
            function_args.push_back(this->get_c_string_ptr_type());
            
            function_type = llvm::FunctionType::get(this->get_chime_object_ptr_type(), function_args, false);
            
            function_chime_string_create_with_c_string = llvm::Function::Create(function_type, llvm::GlobalValue::ExternalLinkage, "chime_string_create_with_c_string", this->module());
            function_chime_string_create_with_c_string->setCallingConv(llvm::CallingConv::C);
            
            this->set_value_for_identifier("chime_string_create_with_c_string", function_chime_string_create_with_c_string);
        }
        
        llvm::Value* c_string_ptr;
        
        c_string_ptr = this->make_constant_string(str);
        
        alloca = this->insert_chime_object_alloca();
        
        call = this->builder()->CreateCall(function_chime_string_create_with_c_string, c_string_ptr, "create string");
        call->setTailCall(false);
        
        this->builder()->CreateStore(call, alloca, false);
        
        return alloca;
    }
    
    void code_generator::make_main(void)
    {
        llvm::LLVMContext&             context = this->module()->getContext();
        const llvm::IntegerType*       int32_type;
        llvm::Type*                    int8_ptr_type;
        std::vector<const llvm::Type*> main_function_args;
        llvm::FunctionType*            main_function_type; 
        llvm::Function*                main_function;
        
        int32_type    = llvm::IntegerType::get(context, 32);
        int8_ptr_type = this->get_c_string_ptr_type();
        
        main_function_args.push_back(int32_type);
        main_function_args.push_back(llvm::PointerType::get(int8_ptr_type, 0));
        
        main_function_type = llvm::FunctionType::get(int32_type, main_function_args, false);
          
        main_function = llvm::Function::Create(main_function_type, llvm::GlobalValue::ExternalLinkage, "main", this->module());
        main_function->setCallingConv(llvm::CallingConv::C);
        
        // function body
        llvm::BasicBlock* label_entry = llvm::BasicBlock::Create(context, "entry", main_function, 0);
        this->builder()->SetInsertPoint(label_entry);
        
        this->call_chime_runtime_initialize();
    }
    
    void code_generator::generate(ast::node* node, const char* module_name)
    {
        std::vector<ast::node*>::iterator i;
        
        assert(node != NULL);
        
        _module = new llvm::Module(module_name, llvm::getGlobalContext());
        
        this->make_main();
        
        for (i=node->children()->begin(); i < node->children()->end(); i++)
        {
            (*i)->codegen(*this);
        }
        
        this->builder()->CreateRet(llvm::ConstantInt::get(*this->get_context(), llvm::APInt(32, 2, false)));
        
        llvm::verifyModule(*this->module(), llvm::PrintMessageAction);
    }
}
