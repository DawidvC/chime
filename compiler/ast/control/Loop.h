// chime: Loop.h

#ifndef LOOP_H
#define LOOP_H

#include "compiler/ast/node.h"

namespace chime
{
    class Loop : public ast::Node
    {
    public:
        static Loop* parse(chime::parser& parser);
        
    public:
        virtual std::string nodeName(void) const;
        
        NodeRef getCondition() const;
        void    setCondition(NodeRef node);
        NodeRef getBody() const;
        void    setBody(NodeRef node);
        
    private:
        ast::NodeRef _condition;
        ast::NodeRef _body;
    };
    
    typedef shared_ptr<Loop> LoopRef;
}
#endif // LOOP_H
