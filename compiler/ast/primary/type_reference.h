// chime: type_reference.h

#ifndef TYPE_REFERENCE_H
#define TYPE_REFERENCE_H

#include "compiler/ast/node.h"

namespace ast
{
    class ParameterSet;
    typedef std::tr1::shared_ptr<ast::ParameterSet> ParameterSetRef;
    
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
        std::string     _identifier;
        ParameterSetRef _parameters;
    };
    
    typedef std::tr1::shared_ptr<ast::type_reference> TypeRef;
}

#endif // TYPE_REFERENCE_H
