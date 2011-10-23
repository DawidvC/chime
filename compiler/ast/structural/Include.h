// chime: Include.h

#ifndef INCLUDE_H
#define INCLUDE_H

#include "compiler/ast/node.h"
#include "compiler/ast/primary/type_reference.h"

namespace chime
{
    class Include : public Node
    {
    public:
        static chime::Include* parse(parser& parser);
    
    public:
        std::string nodeName() const;
        
        TypeRef     getTraitType() const;
        
        llvm::Value* codegen(CodeGenContext& context);
        
    private:
        TypeRef _traitName;
    };
}

#endif // INCLUDE_H
