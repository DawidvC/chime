#include "Switch.h"
#include "Case.h"
#include "compiler/ast/common/CodeBlock.h"

namespace ast
{
    ast::Node* Switch::parse(chime::parser& parser)
    {
        ast::Switch*  node;
        chime::token* t;
        
        parser.next_token("switch");
        
        node = new ast::Switch();
        
        node->setExpression(ast::NodeRef(parser.parse_expression()));
        
        parser.advanceToNextStatement();
        parser.next_token("{");
        parser.advanceToNextStatement();
        
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
                CaseRef caseNode;
                
                caseNode = ast::Case::parse(parser, node->getExpression().get());
                
                if (node->_cases.size() > 0)
                {
                    node->_cases.back()->setElse(caseNode);
                }
                
                node->_cases.push_back(caseNode);
            }
            else if (t->equal_to("else"))
            {
                parser.next_token("else");
                
                if (node->_cases.size() == 0)
                {
                    parser.addError("A switch statement cannot contain only an else statement");
                    
                    return node;
                }
                
                node->_cases.back()->setElse(ast::CodeBlock::parse(parser));
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
        
        parser.advanceToNextStatement();
        parser.next_token("}");
        parser.advanceToNextStatement();
        
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
        return _cases.back()->getElse();
    }
    
    llvm::Value* Switch::codegen(chime::code_generator& generator)
    {
        std::vector<ast::CaseRef>::iterator it;
        llvm::Function*                     function;
        llvm::BasicBlock*                   endBlock;
        
        function = generator.builder()->GetInsertBlock()->getParent();
        
        endBlock = llvm::BasicBlock::Create(generator.get_context(), "switch.else");
        
        for (it = _cases.begin(); it != _cases.end(); ++it)
        {
            (*it)->codegenConditional(generator, endBlock);
        }
        
        // guard against duplicate terminators again here
        if (!generator.builder()->GetInsertBlock()->getTerminator())
        {
            generator.builder()->CreateBr(endBlock);
        }
        
        function->getBasicBlockList().push_back(endBlock);
        generator.builder()->SetInsertPoint(endBlock);
        
        return NULL;
    }
}
