// chime: code_block.cpp

#include "code_block.h"
#include "compiler/ast/structural/Structural.h"

#include <tr1/memory>

namespace ast
{
    NodeRef CodeBlock::parseNextBlock(chime::parser& parser, bool allowStructural)
    {
        NodeRef node;
        
        // first, advance past endings
        parser.advance_past_ending_tokens();
        
        if (parser.advance_token_if_equals("{"))
        {
            chime::LexicalScopeRef scope;
            
            scope = chime::LexicalScopeRef(new chime::LexicalScope());
            //parser.pushCodeBlock(scope);
            
            node = NodeRef(new ast::CodeBlock(parser, allowStructural));
            
            parser.advance_past_ending_tokens();
            
            parser.next_token("}");
            //parser.popCodeBlock();
        }
        else
        {
            node = NodeRef(parser.parse_without_structural());
        }
        
        return node;
    }
    
    CodeBlock::CodeBlock(chime::parser& parser, bool allowStructural)
    {
        ast::node* node;
        
        while (true)
        {
            parser.advance_past_ending_tokens();
            
            // we've reached the end of the block
            if (parser.look_ahead()->is_ending())
                break;
            
            if (allowStructural)
            {
                node = Structural::parse(parser);
            }
            else
            {
                node = parser.parse_without_structural();
            }
            
            if (!node)
            {
                parser.addError("CodeBlock::CodeBlock: unable to get next node");
                break;
            }
            
            this->addChild(*node);
        }
        
        parser.advance_past_ending_tokens();
    }

    std::string CodeBlock::nodeName() const
    {
        return std::string("code block");
    }

    std::string CodeBlock::stringRepresentation(int depth) const
    {
        std::string                       str;
        std::vector<ast::node*>::iterator i;
        
        str.append(depth*2, ' ');
        str.append(this->nodeName());
        
        for (i = this->children()->begin(); i < this->children()->end(); ++i)
        {
            str.append("\n");
            
            if ((*i) == NULL)
            {
                str.append(depth+1, ' ');
                str.append("(null)\n");
            }
            else
            {
                str.append((*i)->stringRepresentation(depth+1));
            }
        }
        
        return str;
    }
    
    void CodeBlock::addChild(const ast::node& node)
    {
        this->add_child((ast::node*)&node);
    }
    
    ast::node* CodeBlock::childAtIndex(unsigned int i) const
    {
        return this->child_at_index(i);
    }
    
    chime::LexicalScopeRef CodeBlock::getScope() const
    {
        return _scope;
    }
    
    llvm::Value* CodeBlock::codegen(chime::code_generator& generator)
    {
        std::vector<ast::node*>::iterator i;
        
        for (i = this->children()->begin(); i < this->children()->end(); ++i)
        {
            (*i)->codegen(generator);
        }
        
        return NULL;
    }
}