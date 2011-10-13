#include "Next.h"

namespace chime
{
    Node* Next::parse(parser& parser)
    {
        Next* node;
        
        parser.nextToken("next");
        
        node = new Next();
        
        return node;
    }
    
    std::string Next::nodeName() const
    {
        return std::string("Next");
    }
    
    llvm::Value* Next::codegen(chime::code_generator& generator)
    {
        llvm::BasicBlock* block;
        
        assert(!generator.builder()->GetInsertBlock()->getTerminator() && "A next statement cannot occur in a block that has already been terminated");
        
        block = generator.getCurrentScope()->getStartBlock();
        assert(block && "A next statement must be inside a loop construct");
        
        // here, we need to branch to the starting block of the inner-most loop
        generator.builder()->CreateBr(block);
        
        return NULL;
    }
}
