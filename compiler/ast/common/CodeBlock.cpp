// chime: CodeBlock.cpp

#include "CodeBlock.h"
#include "compiler/ast/structural/Structural.h"

namespace ast
{
    NodeRef CodeBlock::parse(chime::parser& parser, bool allowStructural)
    {
        CodeBlockRef block;
        bool         openBraceFound;
        
        parser.advanceToNextStatement();
        
        block = CodeBlockRef(new ast::CodeBlock());
        
        openBraceFound = parser.advance_token_if_equals("{");
        
        if (!openBraceFound && allowStructural)
        {
            parser.addError("Missing open brace in structrual context");
            return block;
        }
        
        while (true)
        {
            Node* node;
            
            parser.advanceToNextStatement();
            
            if (parser.look_ahead()->isBlockEnding())
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
                parser.addError("CodeBlock::parse: unable to get next node");
                break;
            }
            
            block->addChild(*node);
        }
        
        if (openBraceFound)
            parser.next_token("}");
        
        return std::tr1::static_pointer_cast<Node>(block);
    }
    
    // NodeRef CodeBlock::parseNextBlock(chime::parser& parser, bool allowStructural)
    // {
    //     NodeRef node;
    //     bool    openBraceFound;
    //     
    //     // first, advance past endings
    //     parser.advance_past_ending_tokens();
    //     
    //     openBraceFound = parser.advance_token_if_equals("{");
    //     
    //     if (openBraceFound || allowStructural)
    //     {
    //         node = NodeRef(new ast::CodeBlock(parser, allowStructural));
    //         
    //         parser.advance_past_ending_tokens();
    //         
    //         if (openBraceFound)
    //             parser.next_token("}");
    //     }
    //     else
    //     {
    //         node = NodeRef(parser.parse_without_structural());
    //     }
    //     
    //     return node;
    // }
    // 
    // CodeBlock::CodeBlock(chime::parser& parser, bool allowStructural)
    // {
    //     ast::node* node;
    //     
    //     while (true)
    //     {
    //         parser.advance_past_ending_tokens();
    //         
    //         // we've reached the end of the block
    //         if (parser.look_ahead()->isBlockEnding())
    //             break;
    //         
    //         if (allowStructural)
    //         {
    //             node = Structural::parse(parser);
    //         }
    //         else
    //         {
    //             node = parser.parse_without_structural();
    //         }
    //         
    //         if (!node)
    //         {
    //             parser.addError("CodeBlock::CodeBlock: unable to get next node");
    //             break;
    //         }
    //         
    //         this->addChild(*node);
    //     }
    //     
    //     parser.advance_past_ending_tokens();
    // }

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