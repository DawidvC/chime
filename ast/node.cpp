#include "node.h"
#include <string>
#include <assert.h>
#include "../lexer/token.h"
#import "structural/implementation.h"
#import "structural/import.h"
#import "primary/entity_reference.h"
#import "operators/binary_operator.h"

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
        assert(new_child != NULL);
        
        _children->push_back(new_child);
    }
    
    std::vector<ast::node*>* node::children(void) const
    {
        return _children;
    }
    
    ast::node* node::child_at_index(unsigned int i) const
    {
        assert(i < this->child_count());
        
        return (*_children)[i];
    }
    
    unsigned int node::child_count() const
    {
        return (int)_children->size();
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
                str.append((*i)->string_representation(depth+1));
            }
        }
        
        return str;
    }
    void node::print(void)
    {
        fprintf(stderr, "%s\n", this->string_representation().c_str());
    }
    
    llvm::Value* node::codegen(void)
    {
        assert(false);
        
        return NULL;
    }
}
