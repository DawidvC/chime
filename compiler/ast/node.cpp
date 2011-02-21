#include "node.h"
#include <string>

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
    
    void node::add_child(ast::node* new_child)
    {
        if (new_child == NULL)
            throw "nope!";
        
        _children->push_back(new_child);
    }
    
    std::vector<ast::node*>* node::children(void) const
    {
        return _children;
    }
    
    ast::node* node::child_at_index(unsigned int i) const
    {
        return (*_children)[i];
    }
    
    std::string node::node_name(void)
    {
        return std::string("node");
    }
    
    std::string node::to_string(void)
    {
        return this->node_name();
    }
    std::string node::string_representation(int depth)
    {
        std::string str;
        std::vector<ast::node*>::iterator i;
        
        str.append(depth,' ');
        str.append(this->to_string());
        
        for (i=_children->begin(); i < _children->end(); i++)
        {
            str.append("\n");
            str.append((*i)->string_representation(depth+1));
        }
    
        return str;
    }
    void node::print(void)
    {
        printf("%s\n", this->string_representation().c_str());
    }
}
