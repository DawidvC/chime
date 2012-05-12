// chime: ArrayLiteral.h

#ifndef ARRAY_LITERAL_H
#define ARRAY_LITERAL_H

#include "Literal.h"

namespace chime
{
    class ArrayLiteral : public Literal
    {
    public:
        static Node* parse(parser& parser);
        
    public:
        virtual std::string nodeName() const;
        
        llvm::Value* codegen(CodeGenContext& context);
    };
}

#endif // ARRAY_LITERAL_H
