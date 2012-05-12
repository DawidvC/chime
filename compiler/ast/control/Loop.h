// chime: Loop.h

#ifndef LOOP_H
#define LOOP_H

#include "compiler/ast/LocalScopedNode.h"

namespace chime
{
    class Loop : public LocalScopedNode
    {
    public:
        static Loop* parse(chime::parser& parser);
        
    public:
        virtual std::string nodeName(void) const;
        virtual std::string stringRepresentation(int depth=0) const;
        
        NodeRef getCondition() const;
        void    setCondition(NodeRef node);
        NodeRef getBody() const;
        void    setBody(NodeRef node);
        
        virtual llvm::Value* codegen(code_generator& generator);
        
    private:
        NodeRef _condition;
        NodeRef _body;
    };
    
    typedef shared_ptr<Loop> LoopRef;
}
#endif // LOOP_H
