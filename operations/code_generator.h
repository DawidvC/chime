// node.h
//

#ifndef CODE_GENERATOR
#define CODE_GENERATOR

#include <map>

#include "llvm/DerivedTypes.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/IRBuilder.h"

#include "../ast/node.h"

namespace chime
{
    class code_generator
    {
    public:
        code_generator(void);
        virtual ~code_generator(void);
        
        llvm::IRBuilder<>* builder(void) const;
        llvm::Module*      module(void) const;
        
        llvm::Function* define_chime_runtime_initialize(void);
        
        void generate(ast::node* node, const char* module_name);
        
    protected:
        llvm::Module*                        _module;
        llvm::IRBuilder<>*                   _builder;
        std::map<std::string, llvm::Value*>* _scope_values;
        
        void make_main(void);
    };
}

#endif
