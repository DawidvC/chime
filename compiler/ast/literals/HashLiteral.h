// chime: HashLiteral.h

#ifndef HASH_LITERAL_H
#define HASH_LITERAL_H

#include "Literal.h"

namespace chime
{
    class HashLiteral : public Literal
    {
    public:
        static Node* parse(parser& parser);
        
    public:
        virtual std::string nodeName() const;
        
        llvm::Value* codegen(CodeGenContext& context);
    };
}

#endif // HASH_LITERAL_H
