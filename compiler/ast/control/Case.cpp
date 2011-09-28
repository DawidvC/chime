#include "Case.h"
#include "compiler/ast/common/code_block.h"

namespace ast
{
    ast::CaseRef Case::parse(chime::parser& parser)
    {
        ast::Case* node;
        
        parser.next_token("case");
        
        node = new ast::Case();
        node->setCondition(ast::NodeRef(parser.parse_expression()));
        
        parser.advance_past_ending_tokens();
        
        node->setBody(CodeBlock::parseNextBlock(parser));
        
        parser.advance_past_ending_tokens();
        
        return ast::CaseRef(node);
    }
    
    std::string Case::nodeName(void) const
    {
        return std::string("Case");
    }
    
    ast::NodeRef Case::getCondition() const
    {
        return _condition;
    }
    
    void Case::setCondition(ast::NodeRef node)
    {
        _condition = node;
    }
    
    ast::NodeRef Case::getBody(void) const
    {
        return _body;
    }
    void Case::setBody(ast::NodeRef node)
    {
        _body = node;
    }
}
