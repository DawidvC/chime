// chime: Break.h

#ifndef BREAK_H
#define BREAK_H

#include "compiler/ast/node.h"

namespace chime
{
    class Break : public Node
    {
    public:
        static Node* parse(parser& parser);
        
    public:
        std::string nodeName() const;
        
        llvm::Value* codegen(chime::code_generator& generator);
    };
}

#endif // BREAK_H
