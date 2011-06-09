#ifndef TYPE_REFERENCE
#define TYPE_REFERENCE

#include "ast/primary/entity_reference.h"

namespace ast
{
    class type_reference : public entity_reference
    {
    public:
        type_reference(chime::parser* parser);
        
        virtual std::string node_name(void);
        virtual std::string to_string(void);
        
        virtual std::string identifier() const;
        virtual void identifier(std::string s);
        
        llvm::Value* codegen(chime::code_generator& generator);
        
    protected:
        std::string _identifier;
        
        void parse_parameters(chime::parser* parser);
    };
}

#endif
