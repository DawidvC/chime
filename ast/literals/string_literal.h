#ifndef STRING_LITERAL
#define STRING_LITERAL

#include <string>
#include "ast/node.h"

namespace ast
{
    class string_literal : public node
    {
    public:
        string_literal(chime::parser* parser);
        
        virtual std::string node_name(void) const;
        virtual std::string to_string(void) const;
        
        std::string value(void) const;
        void value(std::string v);
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
        
    private:
        std::string _value;
    };
}

#endif
