#include "node.h"
#include <string>
#include <assert.h>
#include "lexer/token.h"
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
    
    void node::replace_child_at_index(unsigned int i, ast::node* new_child)
    {
        (*_children)[i] = new_child;
    }
    
    unsigned int node::child_count() const
    {
        return (int)_children->size();
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
        //assert(false);
        
        return NULL;
    }
}
