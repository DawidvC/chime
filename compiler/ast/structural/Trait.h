// chime: Trait.h

#ifndef TRAIT_H
#define TRAIT_H

#include "compiler/ast/ScopedNode.h"
#include "compiler/ast/primary/type_reference.h"

#include <llvm/Function.h>

namespace chime
{
    class Trait : public ast::ScopedNode
    {
    public:
        static Trait* parse(parser& parser);
    
    public:
        virtual std::string nodeName() const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        TypeRef         getTypeRef() const;
        NodeRef         getBody() const;
        
        std::string     getIdentifier() const;
        
        ast::Variable*  createVariable(const std::string& identifier);
        
        llvm::Function* createInitFunction(CodeGenContext& context);
        llvm::Value*    codegen(CodeGenContext& context);
        
    protected:
        TypeRef _typeRef;
        NodeRef _bodyBlock;
    };
}

#endif // TRAIT_H
