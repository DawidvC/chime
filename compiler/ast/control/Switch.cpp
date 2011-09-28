#include "Switch.h"
#include "Case.h"
#include "compiler/ast/common/code_block.h"

namespace ast
{
    ast::Node* Switch::parse(chime::parser& parser)
    {
        ast::Switch*  node;
        chime::token* t;
        
        parser.next_token("switch");
        
        node = new ast::Switch();
        
        node->setExpression(ast::NodeRef(parser.parse_expression()));
        
        parser.advance_past_ending_tokens();
        parser.next_token("{");
        parser.advance_past_ending_tokens();
        
        while (true)
        {
            t = parser.look_ahead();
            if (t->empty())
            {
                parser.addError("Switch statement malformed");
                return node;
            }
            
            if (t->equal_to("case"))
            {
                node->_cases.push_back(ast::Case::parse(parser));
            }
            else if (t->equal_to("else"))
            {
                parser.next_token("else");
                
                node->_else = CodeBlock::parseNextBlock(parser);
                break;
            }
            else if(t->equal_to("}"))
            {
                break;
            }
            else
            {
                parser.addError("Found an illegal element in a switch body");
                break;
            }
        }
        
        parser.advance_past_ending_tokens();
        parser.next_token("}");
        parser.advance_past_ending_tokens();
        
        return node;
    }
    
    std::string Switch::nodeName(void) const
    {
        return std::string("Switch");
    }
    
    ast::NodeRef Switch::getExpression() const
    {
        return _expression;
    }
    
    void Switch::setExpression(ast::NodeRef expression)
    {
        _expression = expression;
    }
    
    std::vector<ast::CaseRef> Switch::getCases() const
    {
        return _cases;
    }
    
    ast::NodeRef Switch::getElse() const
    {
        return _else;
    }
}
