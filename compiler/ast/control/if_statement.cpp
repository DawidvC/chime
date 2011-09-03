#include "if_statement.h"

#include "compiler/ast/common/code_block.h"

namespace ast
{
    IfStatement::IfStatement(chime::parser& parser, ast::node* body)
    {
        parser.next_token("if");
        
        this->setCondition(ast::NodeRef(parser.parse_expression()));
        
        if (body)
        {
            this->setBody(ast::NodeRef(body));
        }
        else
        {
            this->setBody(CodeBlock::parseNextBlock(parser));
        }
        
        parser.advance_past_ending_tokens();
        
        if (parser.advance_token_if_equals("else"))
        {
            this->setElse(CodeBlock::parseNextBlock(parser));
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
        llvm::Value*      conditionValue;
        llvm::Value*      thenValue;
        llvm::Value*      elseValue;
        llvm::Function*   function;
        llvm::BasicBlock* thenBlock;
        llvm::BasicBlock* elseBlock;
        llvm::BasicBlock* endBlock;
        
        // codegen the condition...
        conditionValue = this->getCondition()->codegen(generator);
        
        // ... and wrap it up in a conditional test
        conditionValue = generator.createCondition(conditionValue);
        
        function = generator.builder()->GetInsertBlock()->getParent();
        
        thenBlock  = llvm::BasicBlock::Create(generator.get_context(), "if.then", function);
        elseBlock  = llvm::BasicBlock::Create(generator.get_context(), "if.else");
        endBlock   = llvm::BasicBlock::Create(generator.get_context(), "if.end");
        
        generator.builder()->CreateCondBr(conditionValue, thenBlock, elseBlock);
        
        // setup the then block
        generator.builder()->SetInsertPoint(thenBlock);
        
        thenValue = this->getBody()->codegen(generator);
        
        // branch to the end, unless we've encountered another terminator
        // in the process of codegen (like a return statement)
        if (!generator.builder()->GetInsertBlock()->getTerminator())
        {
            generator.builder()->CreateBr(endBlock);
        }
        
        function->getBasicBlockList().push_back(elseBlock);
        generator.builder()->SetInsertPoint(elseBlock);
        
        // codegen the else block.  If there is no else, we just have it
        // branch right to the end
        elseValue = NULL;
        if (this->getElse())
        {
            elseValue = this->getElse()->codegen(generator);
        }
        
        // guard against duplicate terminators again here
        if (!generator.builder()->GetInsertBlock()->getTerminator())
        {
            generator.builder()->CreateBr(endBlock);
        }
        
        elseBlock = generator.builder()->GetInsertBlock();
        
        function->getBasicBlockList().push_back(endBlock);
        generator.builder()->SetInsertPoint(endBlock);
        
        return NULL;
    }
    
    ast::NodeRef IfStatement::getCondition(void) const
    {
        return _condition;
    }
    void IfStatement::setCondition(ast::NodeRef node)
    {
        _condition = node;
    }
    ast::NodeRef IfStatement::getBody(void) const
    {
        return _body;
    }
    void IfStatement::setBody(ast::NodeRef node)
    {
        _body = node;
    }
    ast::NodeRef IfStatement::getElse(void) const
    {
        return _else;
    }
    void IfStatement::setElse(ast::NodeRef node)
    {
        _else = node;
    }
    
    
}
