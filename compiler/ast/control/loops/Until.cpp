#include "Until.h"
#include "compiler/ast/common/CodeBlock.h"

namespace chime
{
    Until* Until::parse(chime::parser& parser)
    {
        Until* node;
        
        parser.nextToken("until");
        
        node = new Until();
        
        node->setCondition(parser.parseExpression());
        
        parser.advanceToNextStatement();
        
        node->setBody(ast::CodeBlock::parseBlockWithOptionalBraces(parser));
        
        parser.advanceToNextStatement();
        
        return node;
    }
    
    std::string Until::nodeName(void) const
    {
        return std::string("Until");
    }
    
    llvm::Value* Until::codegen(chime::code_generator& generator)
    {
        return NULL;
    }
}
