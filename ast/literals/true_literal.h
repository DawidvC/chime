#ifndef TRUE_LITERAL
#define TRUE_LITERAL

#include "ast/node.h"

namespace ast
{
    class true_literal : public node
    {
    public:
        true_literal(chime::parser* parser);
        
        virtual std::string node_name(void);
        virtual std::string to_string(void);
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
    };
}

#endif
