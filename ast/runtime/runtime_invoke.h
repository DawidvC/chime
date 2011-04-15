#include <string>
#include "ast/node.h"
#include "ast/primary/entity_reference.h"

#ifndef RUNTIME_INVOKE
#define RUNTIME_INVOKE

namespace ast
{
    class runtime_invoke : public node
    {
    public:
        runtime_invoke();
        virtual ~runtime_invoke();
        
        virtual std::string node_name(void);
        virtual std::string to_string(void);
        
        ast::node* target() const;
        void target(ast::node* node);
        std::string property_name() const;
        void property_name(std::string new_name);
        
        llvm::Value* codegen(chime::code_generator& generator);
        
    protected:
        ast::node*  _target;
        std::string _property_name;
    };
}

#endif
