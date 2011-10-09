#include "While.h"
#include "compiler/ast/common/CodeBlock.h"

namespace ast
{
    ast::While* While::parse(chime::parser& parser)
    {
        ast::While* whileNode;
        
        parser.nextToken("while");
        
        whileNode = new While();
        
        whileNode->setCondition(parser.parseExpression());
        
        parser.advanceToNextStatement();
        
        whileNode->setBody(CodeBlock::parseBlockWithOptionalBraces(parser));
        
        parser.advanceToNextStatement();
        
        return whileNode;
    }
    
    NodeRef While::getCondition() const
    {
        return _condition;
    }
    
    void While::setCondition(NodeRef node)
    {
        _condition = node;
    }
    
    NodeRef While::getBody(void) const
    {
        return _body;
    }
    void While::setBody(NodeRef node)
    {
        _body = node;
    }
    
    llvm::Value* While::codegen(chime::code_generator& generator)
    {
        return NULL;
    }
}