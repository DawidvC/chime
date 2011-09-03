#ifndef ENTITY_REFERENCE_NODE
#define ENTITY_REFERENCE_NODE

#include "compiler/ast/node.h"

namespace ast
{
    class entity_reference : public node
    {
    public:
        static ast::node* parse(chime::parser& parser);
    
    public:
        entity_reference(chime::parser* parser);
        virtual ~entity_reference();
        
        virtual std::string nodeName(void) const;
        virtual std::string to_string(void) const;
        
        virtual std::string identifier() const;
        virtual void identifier(std::string s);
        
        llvm::Value* codegen(chime::code_generator& generator);
        
    protected:
        std::string _identifier;
    };
    
    typedef std::tr1::shared_ptr<ast::entity_reference> EntityRef;
}

#endif
