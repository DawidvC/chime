#include "if_statement.h"
#include "compiler/ast/common/CodeBlock.h"

namespace ast
{
    IfStatement::IfStatement(chime::parser& parser, ast::node* body)
    {
        parser.next_token("if");
        
        this->setCondition(ast::NodeRef(parser.parse_expression()));
        
        parser.advanceToNextStatement();
        
        if (body)
        {
            this->setBody(ast::NodeRef(body));
        }
        else
        {
            this->setBody(CodeBlock::parseBlockWithOptionalBraces(parser));
        }
        
        parser.advanceToNextStatement();
        
        if (parser.advance_token_if_equals("else"))
        {
            this->setElse(CodeBlock::parseBlockWithOptionalBraces(parser));
        }
    }
    
    std::string IfStatement::nodeName(void) const
    {
        return std::string("if statement");
    }
    
    std::string IfStatement::stringRepresentation(int depth) const
    {
        std::string str;
        
        str.append(depth*2, ' ');
        str.append(this->nodeName());
        
        str.append("\n");
        str.append((depth+1)*2, ' ');
        str.append("- condition:\n");
        str.append(this->getCondition()->stringRepresentation(depth+2));
        
        str.append("\n");
        str.append((depth+1)*2, ' ');
        str.append("- then:\n");
        str.append(this->getBody()->stringRepresentation(depth+2));
        
        if (this->getElse())
        {
            str.append("\n");
            str.append((depth+1)*2, ' ');
            str.append("- else:\n");
            str.append(this->getElse()->stringRepresentation(depth+2));
        }
        
        return str;
    }
    
    llvm::Value* IfStatement::codegen(chime::code_generator& generator)
    {
        llvm::Function*   function;
        llvm::BasicBlock* endBlock;
        llvm::Value*      value;
        
        function = generator.builder()->GetInsertBlock()->getParent();
        
        endBlock = llvm::BasicBlock::Create(generator.get_context(), "if.end");
        
        value = this->codegenConditional(generator, endBlock);
        
        // guard against duplicate terminators again here
        if (!generator.builder()->GetInsertBlock()->getTerminator())
        {
            generator.builder()->CreateBr(endBlock);
        }
        
        function->getBasicBlockList().push_back(endBlock);
        generator.builder()->SetInsertPoint(endBlock);
        
        return value;
    }
}
