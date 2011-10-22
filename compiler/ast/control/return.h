// chime:: Return.h

#ifndef RETURN_H
#define RETURN_H

#include "compiler/ast/node.h"

namespace chime
{
    class Return : public Node
    {
    public:
        Return(parser& parser);
        
        virtual std::string nodeName(void) const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        ast::NodeRef getReturnValue() const;
        
        virtual llvm::Value* codegen(CodeGenContext& generator);
    
    private:
        ast::NodeRef _returnValue;
    };
}

#endif // RETURN_H
