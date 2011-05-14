#ifndef BOOLEAN_LITERAL
#define BOOLEAN_LITERAL

#include "ast/node.h"

namespace ast
{
    class boolean_literal : public node
    {
    public:
        boolean_literal(chime::parser* parser);
        
        virtual std::string node_name(void);
        virtual std::string to_string(void);
        
        unsigned char value(void) const;
        void          value(unsigned char v);
        
        virtual llvm::Value* codegen(chime::code_generator& generator);
    
    protected:
        unsigned char _value;
    };
}

#endif
