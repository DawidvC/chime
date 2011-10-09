// chime: While.h

#ifndef WHILE_H
#define WHILE_H

#include "compiler/ast/control/Loop.h"

namespace chime
{
    class While : public Loop
    {
    public:
        static While* parse(chime::parser& parser);
        
    public:
        std::string nodeName(void) const;
        
        llvm::Value* codegen(chime::code_generator& generator);
    };
    
    typedef shared_ptr<While> WhileRef;
}
#endif // WHILE_H
