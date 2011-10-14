#include "Conditional.h"
#include "compiler/ast/common/CodeBlock.h"
#include "compiler/ast/control/if_statement.h"
#include "compiler/ast/control/Unless.h"

namespace chime
{
    Node* Conditional::parse(parser& parser, Node* body)
    {
        Conditional* node;
        
        node = NULL;
        if (parser.advanceTokenIfEqual("if"))
        {
            node = new IfStatement();
        }
        else if (parser.advanceTokenIfEqual("unless"))
        {
            node = new Unless();
        }
        else
        {
            parser.addError("Expecting an 'if' or 'unless' here");
            return node;
        }
        
        node->setCondition(parser.parseExpression());
        
        parser.advanceToNextStatement();
        
        if (body)
        {
            ast::CodeBlockRef block;
            
            block = ast::CodeBlockRef(new ast::CodeBlock());
            block->addChild(*body);
            
            node->setBody(block);
        }
        else
        {
            node->setBody(ast::CodeBlock::parseBlockWithOptionalBraces(parser));
        }
        
        parser.advanceToNextStatement();
        
        // no else statements allowed if a body is specified
        if (body)
            return node;
        
        if (parser.advanceTokenIfEqual("else"))
        {
            node->setElse(ast::CodeBlock::parseBlockWithOptionalBraces(parser));
        }
        
        return node;
    }
    
    Node* Conditional::parseTailing(chime::parser& parser, ast::Node* node)
    {
        if (!parser.look_ahead()->isConditional())
        {
            return node;
        }
        
        return Conditional::parse(parser, node);
    }
    
    std::string Conditional::stringRepresentation(int depth) const
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
    
    NodeRef Conditional::getCondition() const
    {
        return _condition;
    }
    
    void Conditional::setCondition(NodeRef node)
    {
        _condition = node;
    }
    
    NodeRef Conditional::getBody(void) const
    {
        return _body;
    }
    void Conditional::setBody(NodeRef node)
    {
        _body = node;
    }
    
    NodeRef Conditional::getElse(void) const
    {
        return _else;
    }
    void Conditional::setElse(NodeRef node)
    {
        _else = node;
    }
    
    llvm::Value* Conditional::codegenConditional(code_generator& generator, llvm::BasicBlock* endBlock, bool negate)
    {
        llvm::Value*      conditionValue;
        llvm::Function*   function;
        llvm::BasicBlock* thenBlock;
        llvm::BasicBlock* elseBlock;
        
        // codegen the condition...
        conditionValue = this->getCondition()->codegen(generator);
        
        // ... and wrap it up in a conditional test
        conditionValue = generator.createCondition(conditionValue);
        
        function = generator.builder()->GetInsertBlock()->getParent();
        
        thenBlock = llvm::BasicBlock::Create(generator.get_context(), "conditional.then", function);
        elseBlock = llvm::BasicBlock::Create(generator.get_context(), "conditional.else");
        
        if (negate)
            generator.builder()->CreateCondBr(conditionValue, elseBlock, thenBlock);
        else
            generator.builder()->CreateCondBr(conditionValue, thenBlock, elseBlock);
        
        // setup the then block
        generator.builder()->SetInsertPoint(thenBlock);
        
        this->getBody()->codegen(generator);
        
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
        if (this->getElse())
        {
            this->getElse()->codegen(generator);
        }
        
        return NULL;
    }
}
