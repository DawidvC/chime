// chime: Return.h

#ifndef RETURN_H
#define RETURN_H

#include "compiler/ast/node.h"

namespace chime
{
    class Return : public Node
    {
    public:
        static Return* parse(parser& parser);
        
    public:
        virtual std::string nodeName() const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        NodeRef getReturnValue() const;
        
        virtual llvm::Value* codegen(CodeGenContext& generator);
    
    private:
        NodeRef _returnValue;
    };
}

#endif // RETURN_H
