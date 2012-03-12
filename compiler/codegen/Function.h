// chime: function.h

#ifndef CHIME_FUNCTION_H
#define CHIME_FUNCTION_H

#include <string>
#include <vector>

#include <llvm/DerivedTypes.h>
#include <llvm/Module.h>

namespace llvm
{
    typedef ArrayRef<Value*> ValueArrayRef;
}

namespace chime
{
    class Function
    {
    public:
        static llvm::Type* getVoidType(llvm::Module* module);
        static llvm::Type* getVoidPtrType(llvm::Module* module);
        static llvm::Type* getCStringPointerType(llvm::Module* module);
        
        static Function    voidReturnNoArgsFunction(llvm::Module* module, const std::string& name);
        
    public:
        Function(llvm::Module* module, const std::string& name);
        Function(llvm::Module* module, const std::string& name, llvm::Type* returnType);
        
        void addArgument(llvm::Type* argumentType);
        void addCStringPtrArgument();
        void addObjectPtrArgument();
        
        llvm::FunctionType* type() const;
        
        std::string name() const;
        void        setName(const std::string& newName);
        
        void setReturnType(llvm::Type* type);
        
        llvm::Function* create() const;
        
    private:
        llvm::Module*            _module;
        std::string              _name;
        llvm::Type*              _returnType;
        std::vector<llvm::Type*> _arguments;
    };
}

#endif // CHIME_FUNCTION_H
