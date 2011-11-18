// implementation.h

#ifndef IMPLEMENTATION_NODE_H
#define IMPLEMENTATION_NODE_H

#include "compiler/ast/ScopedNode.h"
#include "compiler/ast/structural/Type.h"
#include "compiler/ast/CodeBlock.h"

#include <llvm/Function.h>

namespace ast
{
    class Implementation : public ScopedNode
    {
    public:
        static Implementation* parse(chime::parser& parser);
    
    public:
        virtual std::string nodeName(void) const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        chime::TypeRef  getTypeRef() const;
        void            setTypeRef(chime::TypeRef node);
        chime::TypeRef  getSuperclass() const;
        void            setSuperclass(chime::TypeRef node);
        NodeRef         getBody(void) const;
        
        std::string     getIdentifier() const;
        
        chime::Variable* createVariable(const std::string& identifier);
        bool             allowsStructuralElements() const;
        
        llvm::Function* createInitFunction(chime::code_generator& generator);
        llvm::Value*    codegen(chime::code_generator& generator);
        
    protected:
        chime::TypeRef _typeRef;
        chime::TypeRef _superclass;
        NodeRef        _bodyBlock;
    };
}

#endif // IMPLEMENTATION_NODE_H
