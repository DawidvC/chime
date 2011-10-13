#include "Break.h"

namespace chime
{
    Node* Break::parse(parser& parser)
    {
        Break* node;
        
        parser.nextToken("break");
        
        node = new Break();
        
        return node;
    }
    
    std::string Break::nodeName(void) const
    {
        return std::string("Break");
    }
        
    llvm::Value* Break::codegen(chime::code_generator& generator)
    {
        llvm::BasicBlock* block;
        
        // if the block has already been terminated, skip this
        if (generator.builder()->GetInsertBlock()->getTerminator())
            return NULL;
        
        block = generator.getCurrentScope()->getEndBlock();
        assert(block && "A break statement must be inside a loop construct");
        
        // here, we need to branch to the end block of the inner-most loop
        generator.builder()->CreateBr(block);
        
        return NULL;
    }
}
