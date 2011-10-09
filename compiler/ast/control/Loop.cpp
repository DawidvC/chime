#include "Loop.h"
#include "loops/LoopUntil.h"
#include "loops/LoopWhile.h"
#include "compiler/ast/common/CodeBlock.h"

namespace chime
{
    Loop* Loop::parse(chime::parser& parser)
    {
        Token*  t;
        Loop*   node;
        NodeRef body;
        
        parser.nextToken("loop");
        
        node = NULL;
        body = ast::CodeBlock::parseBlockWithOptionalBraces(parser);
        
        t = parser.look_ahead();
        if (t->equal_to("while"))
        {
            parser.nextToken("while");
            
            node = new LoopWhile();
            
            parser.advanceToNextStatement();
            
            node->setCondition(parser.parseExpression());
        }
        else if (t->equal_to("until"))
        {
            parser.nextToken("until");
            
            node = new LoopUntil();
            
            parser.advanceToNextStatement();
            
            node->setCondition(parser.parseExpression());
        }
        else
        {
            node = new Loop();
        }
        
        node->setBody(body);
        
        return node;
    }
    
    std::string Loop::nodeName(void) const
    {
        return std::string("Loop");
    }
    
    NodeRef Loop::getCondition() const
    {
        return _condition;
    }
    
    void Loop::setCondition(NodeRef node)
    {
        _condition = node;
    }
    
    NodeRef Loop::getBody(void) const
    {
        return _body;
    }
    void Loop::setBody(NodeRef node)
    {
        _body = node;
    }
}