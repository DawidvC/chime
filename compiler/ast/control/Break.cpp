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
        
        assert(!generator.builder()->GetInsertBlock()->getTerminator() && "A break statement cannot occur in a block that has already been terminated");
        
        block = generator.getCurrentScope()->getEndBlock();
        assert(block && "A break statement must be inside a loop construct");
        
        // here, we need to branch to the end block of the inner-most loop
        generator.builder()->CreateBr(block);
        
        return NULL;
    }
}
