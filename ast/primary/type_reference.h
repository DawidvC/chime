// chime: type_reference.h

#ifndef TYPE_REFERENCE_H
#define TYPE_REFERENCE_H

#include "ast/node.h"

namespace ast
{
    class type_reference : public node
    {
    public:
        type_reference(chime::parser* parser);
        
        virtual std::string nodeName(void) const;
        virtual std::string toString(void) const;
        
        virtual std::string identifier() const;
        virtual void identifier(std::string s);
        
        llvm::Value* codegen(chime::code_generator& generator);
        
    protected:
        std::string _identifier;
        
        void parse_parameters(chime::parser* parser);
    };
    
    typedef shared_ptr<ast::type_reference> TypeRef;
}

#endif // TYPE_REFERENCE_H
