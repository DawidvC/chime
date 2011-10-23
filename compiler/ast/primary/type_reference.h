// chime: type_reference.h

#ifndef TYPE_REFERENCE_H
#define TYPE_REFERENCE_H

#include "compiler/ast/node.h"

namespace chime
{
    class ParameterSet;
    typedef shared_ptr<ParameterSet> ParameterSetRef;
    
    class type_reference : public Node
    {
    public:
        type_reference(chime::parser* parser);
        
        virtual std::string nodeName(void) const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        virtual std::string identifier() const;
        virtual void identifier(std::string s);
        
        llvm::Value* codegen(chime::code_generator& generator);
        
    protected:
        std::string     _identifier;
        ParameterSetRef _parameters;
    };
    
    typedef type_reference Type;
    typedef shared_ptr<Type> TypeRef;
}

#endif // TYPE_REFERENCE_H
