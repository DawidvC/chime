#ifndef ENTITY_REFERENCE_NODE
#define ENTITY_REFERENCE_NODE

#include "ast/node.h"

namespace ast
{
    class entity_reference : public node
    {
    public:
        entity_reference();
        entity_reference(chime::parser* parser);
        virtual ~entity_reference();
        
        virtual std::string node_name(void) const;
        virtual std::string to_string(void) const;
        
        virtual std::string identifier() const;
        virtual void identifier(std::string s);
        
        llvm::Value* codegen(chime::code_generator& generator);
        
    protected:
        std::string _identifier;
    };
}

#endif
