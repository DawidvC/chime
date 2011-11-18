// chime: Type.h

#ifndef TYPE_H
#define TYPE_H

#include "compiler/ast/node.h"

namespace chime
{
    // forward declaration
    class Parameter;
    typedef shared_ptr<Parameter> ParameterRef;
    
    class Type : public Node
    {
    public:
        Type(chime::parser* parser);
        
        virtual std::string nodeName() const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        std::string identifier() const;
        void        setIdentifier(const std::string& s);
        
        llvm::Value* codegen(chime::code_generator& generator);
        
    private:
        std::string               _identifier;
        std::vector<ParameterRef> _parameters;
    };
    
    typedef shared_ptr<Type> TypeRef;
}

#endif // TYPE_H
