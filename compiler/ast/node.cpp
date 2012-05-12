#include "node.h"
#include "compiler/lexer/token.h"

#include <stdio.h>

namespace chime
{
    node::node()
    {
        _children = new std::vector<Node*>();
    }
    
    node::~node()
    {
        delete _children;
    }
    
    void node::addChild(Node* n)
    {
        assert(n != NULL);
        
        _children->push_back(n);
    }
    
    void node::add_child(Node* new_child)
    {
        this->addChild(new_child);
    }
    
    std::vector<Node*>* node::children(void) const
    {
        return _children;
    }
    
    Node* node::childAtIndex(unsigned int i) const
    {
        assert(i < this->childCount());
        
        return (*_children)[i];
    }
    
    Node* node::child_at_index(unsigned int i) const
    {
        return this->childAtIndex(i);
    }
    
    void node::replace_child_at_index(unsigned int i, Node* new_child)
    {
        (*_children)[i] = new_child;
    }
    
    unsigned int node::childCount() const
    {
        return _children->size();
    }
    
    unsigned int node::child_count() const
    {
        return this->childCount();
    }
    
    std::string node::node_name(void) const
    {
        return this->nodeName();
    }
    
    std::string node::nodeName(void) const
    {
        return std::string("node");
    }
    
    std::string node::to_string(void) const
    {
        return this->node_name();
    }
    std::string node::stringRepresentation(int depth) const
    {
        std::string                  str;
        std::vector<Node*>::iterator i;
        
        str.append(depth*2, ' ');
        str.append(this->to_string());
        
        for (i=_children->begin(); i < _children->end(); i++)
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
    void node::print(void)
    {
        fprintf(stderr, "%s\n", this->stringRepresentation().c_str());
    }
    
    bool node::isVariable() const
    {
        return false;
    }
    
    bool node::isSelf() const
    {
        return false;
    }
    
    bool node::requiresRelease() const
    {
        return false;
    }
    
    llvm::Value* node::codegen(chime::code_generator& generator)
    {
        return NULL;
    }
}
