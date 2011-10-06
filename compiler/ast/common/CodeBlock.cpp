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
    
    NodeRef CodeBlock::parseBlockWithOptionalBraces(chime::parser& parser)
    {
        CodeBlockRef block;
        
        parser.advanceToNextStatement();
        
        // only use a code block if we encountered an open brace
        if (parser.look_ahead()->equal_to("{"))
        {
            return CodeBlock::parse(parser);
        }
        
        block = CodeBlockRef(new CodeBlock());
        block->addChild(*parser.parse_without_structural());
            
        return block;
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