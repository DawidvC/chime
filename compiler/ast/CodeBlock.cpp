// chime: CodeBlock.cpp

#include "CodeBlock.h"
#include "compiler/ast/structural/Structural.h"

namespace chime
{
    NodeRef CodeBlock::parse(chime::parser& parser, bool allowStructural)
    {
        CodeBlockRef block;
        bool         openBraceFound;
        
        parser.advanceToNextStatement();
        
        block = CodeBlockRef(new CodeBlock());
        
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
                node = chime::Structural::parse(parser);
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
    
    CodeBlock::CodeBlock()
    {
        _deferToParent = false;
    }
    
    std::string CodeBlock::nodeName() const
    {
        return std::string("code block");
    }
    
    std::string CodeBlock::stringRepresentation(int depth) const
    {
        std::string                  str;
        std::vector<Node*>::iterator i;
        
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
    
    std::string CodeBlock::getIdentifier() const
    {
        for (ScopedNode* node = this->parent(); node != NULL; node = node->parent())
        {
            // we can just walk through our parents looking for a node that defines "self".  That
            // should be sufficient
            if (node->selfObjectPtr())
            {
                return node->getIdentifier();
            }
        }
        
        assert(0 && "Unable to determine a good identifier for a code block - this will cause link failures");
        
        return this->nodeName();
    }
    
    bool CodeBlock::deferToParent() const
    {
        return _deferToParent;
    }
    void CodeBlock::setDeferToParent(bool value)
    {
        _deferToParent = value;
    }
    
    void CodeBlock::addChild(const chime::node& node)
    {
        this->add_child((chime::node*)&node);
    }
    
    chime::node* CodeBlock::childAtIndex(unsigned int i) const
    {
        return this->child_at_index(i);
    }
    
    void CodeBlock::addLooseValue(llvm::Value* value)
    {
        if (_deferToParent)
            this->parent()->addLooseValue(value);
        else
            chime::ScopedNode::addLooseValue(value);
    }
    
    void CodeBlock::removeLooseValue(llvm::Value* value)
    {
        if (_deferToParent)
            this->parent()->removeLooseValue(value);
        else
            chime::ScopedNode::removeLooseValue(value);
    }
    
    llvm::Value* CodeBlock::getValueForIdentifier(const std::string& identifier)
    {
        if (_deferToParent)
            return this->parent()->getValueForIdentifier(identifier);
        
        return chime::ScopedNode::getValueForIdentifier(identifier);
    }
    
    void CodeBlock::setValueForIdentifier(const std::string& identifier, llvm::Value* value, bool shouldRelease)
    {
        if (_deferToParent)
            this->parent()->setValueForIdentifier(identifier, value, shouldRelease);
        else
            chime::ScopedNode::setValueForIdentifier(identifier, value, shouldRelease);
    }
    
    llvm::Value* CodeBlock::selfValue(chime::CodeGenContext& context)
    {
        return this->parent()->selfValue(context);
    }
    
    llvm::Value* CodeBlock::selfObjectPtr() const
    {
        return this->parent()->selfObjectPtr();
    }
    
    void CodeBlock::setSelfObjectPtr(llvm::Value* value)
    {
        this->parent()->setSelfObjectPtr(value);
    }
    
    llvm::Value* CodeBlock::classObjectPtr() const
    {
        return this->parent()->classObjectPtr();
    }
    
    void CodeBlock::setClassObjectPtr(llvm::Value* value)
    {
        this->parent()->setClassObjectPtr(value);
    }
    
    llvm::Value* CodeBlock::codegen(chime::code_generator& generator)
    {
        std::vector<chime::Node*>::iterator it;
        
        generator.pushScope(this);
        
        for (it = this->children()->begin(); it < this->children()->end(); ++it)
        {
            (*it)->codegen(generator);
        }
        
        generator.popScope();
        
        return NULL;
    }
}