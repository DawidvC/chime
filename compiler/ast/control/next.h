// chime:: Next.h

#ifndef NEXT_H
#define NEXT_H

#include "compiler/ast/node.h"

namespace chime
{
    class Next : public Node
    {
    public:
        static Node* parse(parser& parser);
        
    public:
        std::string nodeName() const;
        
        llvm::Value* codegen(chime::code_generator& generator);
    };
}
#endif // NEXT_H
