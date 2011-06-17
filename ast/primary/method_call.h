#include <string>
#include "ast/node.h"

#ifndef METHOD_CALL
#define METHOD_CALL

namespace ast
{
    class method_call : public node
    {
    public:
        method_call(chime::parser* parser);
        
        virtual std::string node_name(void) const;
        virtual std::string to_string(void) const;
        
        std::string identifier() const;
        void identifier(std::string s);
        
        llvm::Value* codegen_with_target(llvm::Value* target, chime::code_generator& generator);
        
    protected:
        std::string _identifier;
        
        void parse_arguments(chime::parser* parser);
    };
}

#endif
