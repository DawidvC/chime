#include "While.h"
#include "compiler/ast/common/CodeBlock.h"

namespace chime
{
    While* While::parse(chime::parser& parser)
    {
        While* whileNode;
        
        parser.nextToken("while");
        
        whileNode = new While();
        
        whileNode->setCondition(parser.parseExpression());
        
        parser.advanceToNextStatement();
        
        whileNode->setBody(ast::CodeBlock::parseBlockWithOptionalBraces(parser));
        
        parser.advanceToNextStatement();
        
        return whileNode;
    }
    
    std::string While::nodeName(void) const
    {
        return std::string("While");
    }
    
    llvm::Value* While::codegen(chime::code_generator& generator)
    {
        return NULL;
    }
}