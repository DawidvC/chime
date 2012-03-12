#include "Function.h"

namespace chime
{
    llvm::Type* Function::getVoidType(llvm::Module* module)
    {
        return llvm::Type::getVoidTy(module->getContext());
    }
    
    llvm::Type* Function::getVoidPtrType(llvm::Module* module)
    {
        return llvm::PointerType::get(llvm::IntegerType::get(module->getContext(), 8), 0);
    }
    
    llvm::Type* Function::getCStringPointerType(llvm::Module* module)
    {
        return Function::getVoidPtrType(module);
    }
    
    Function Function::voidReturnNoArgsFunction(llvm::Module* module, const std::string& name)
    {
        return Function(module, name);
    }
    
    Function::Function(llvm::Module* module, const std::string& name) :
        _module(module),
        _name(name)
    {
        _returnType = Function::getVoidType(module);
    }
    
    Function::Function(llvm::Module* module, const std::string& name, llvm::Type* returnType) :
        _module(module),
        _name(name),
        _returnType(returnType)
    {
    }
    
    void Function::addArgument(llvm::Type* argumentType)
    {
        _arguments.push_back(argumentType);
    }
    
    llvm::FunctionType* Function::type() const
    {
        return llvm::FunctionType::get(_returnType, llvm::ArrayRef<llvm::Type*>(_arguments), false);
    }
    
    std::string Function::name() const
    {
        return _name;
    }
    
    void Function::setName(const std::string& newName)
    {
        _name = newName;
    }
    
    void Function::setReturnType(llvm::Type* type)
    {
        _returnType = type;
    }
    
    llvm::Function* Function::create() const
    {
        llvm::Function* function;
        
        function = llvm::Function::Create(this->type(), llvm::GlobalValue::ExternalLinkage, _name.c_str(), _module);
        function->setCallingConv(llvm::CallingConv::C);
        
        return function;
    }
}

