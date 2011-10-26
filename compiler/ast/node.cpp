#include "node.h"
#include "compiler/lexer/token.h"

namespace ast
{
    node::node()
    {
        _children = new std::vector<ast::node*>();
    }
    
    node::~node()
    {
        delete _children;
    }
    
    void node::addChild(ast::node* n)
    {
        assert(n != NULL);
        
        _children->push_back(n);
    }
    
    void node::add_child(ast::node* new_child)
    {
        this->addChild(new_child);
    }
    
    std::vector<ast::node*>* node::children(void) const
    {
        return _children;
    }
    
    ast::node* node::childAtIndex(unsigned int i) const
    {
        assert(i < this->childCount());
        
        return (*_children)[i];
    }
    
    ast::node* node::child_at_index(unsigned int i) const
    {
        return this->childAtIndex(i);
    }
    
    void node::replace_child_at_index(unsigned int i, ast::node* new_child)
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
        std::string str;
        std::vector<ast::node*>::iterator i;
        
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
    
    llvm::Value* node::codegen(chime::code_generator& generator)
    {
        return NULL;
    }
}
