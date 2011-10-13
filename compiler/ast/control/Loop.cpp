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
        
        parser.pushScope(node);
        
        node->setBody(body);
        
        parser.popScope();
        
        return node;
    }
    
    Loop::Loop()
    {
        _startBlock = NULL;
        _endBlock       = NULL;
    }
    
    std::string Loop::nodeName(void) const
    {
        return std::string("Loop");
    }
    
    std::string Loop::stringRepresentation(int depth) const
    {
        std::string str;
        
        str.append(depth*2, ' ');
        str.append("loop");
        
        str.append("\n");
        
        str.append(this->getBody()->stringRepresentation(depth+1));
        
        return str;
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
    
    llvm::Value* Loop::codegen(code_generator& generator)
    {
        llvm::Function* function;
        
        function = generator.builder()->GetInsertBlock()->getParent();
        
        this->setStartBlock(llvm::BasicBlock::Create(generator.getContext(), "loop.body", function));
        this->setEndBlock(llvm::BasicBlock::Create(generator.getContext(), "loop.end", function));
        
        generator.builder()->CreateBr(this->getStartBlock());
        
        generator.builder()->SetInsertPoint(this->getStartBlock());
        
        generator.pushScope(this);
        
        this->getBody()->codegen(generator);
        
        // guard against duplicate terminators again here
        if (!generator.builder()->GetInsertBlock()->getTerminator())
        {
            generator.builder()->CreateBr(this->getStartBlock()); // go back to the beginning
        }
        
        generator.popScope();
        generator.builder()->SetInsertPoint(this->getEndBlock());
        
        return NULL;
    }
    
    llvm::BasicBlock* Loop::getStartBlock() const
    {
        return _startBlock;
    }
    void Loop::setStartBlock(llvm::BasicBlock* block)
    {
        _startBlock = block;
    }
    
    llvm::BasicBlock* Loop::getEndBlock() const
    {
        return _endBlock;
    }
    void Loop::setEndBlock(llvm::BasicBlock* block)
    {
        _endBlock = block;
    }
}
