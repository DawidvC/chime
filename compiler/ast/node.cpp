#include "node.h"
#include <string>
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
        fprintf(stderr, "%s\n", this->string_representation().c_str());
    }
    
    ast::node* node::parse(chime::parser* parser)
    {
        return NULL;
    }
    
    ast::node* construct(chime::parser* parser)
    {
        chime::token* t;
        ast::node*   root;
        
        root = new ast::node();
        
        while (true)
        {
            ast::node* node;
            
            t = parser->look_ahead();
            if (t->empty())
                break;
            
            node = NULL;
            
            if (t->equal_to("import"))
            {
                node = new ast::import(parser);
            }
            // else if (t->equal_to("interface"))
            // {
            //     
            // }
            else if (t->equal_to("implementation"))
            {
                node = new ast::implementation(parser);
            }
            else
            {
                fprintf(stderr, "found something we weren't expecting in construct '%s'\n", t->value.c_str());
                
                break;
            }
            
            if (node)
                root->add_child(node);
        }
        
        return root;
    }
    
    ast::node* construct_expression(chime::parser* parser)
    {
        return NULL;
    }
    
    ast::node* construct_valued_expression(chime::parser* parser)
    {
        chime::token* t;
        ast::node*    node;
        
        node = NULL;
        
        t = parser->look_ahead();
        if (t->is_string())
        {
            throw "construct_valued_expression: found string - unimplemented";
        }
        else if (t->is_entity())
        {
            node = new ast::entity_reference(parser);
        }
        
        // we found the first node, but there could be an operator applied
        // to that node
        t = parser->look_ahead();
        if (t->precedence() > 0)
        {
            node = new ast::binary_operator(parser, node);
        }
        
        return node;
    }
    
    // ast::node* construct_binary_operator(int precedence, ast::node* left_operand)
    // {
    //     ast::binary_operator* node;
    //     ast::node* right_operand;
    //     token* t;
    //     
    //     while ((t = this->look_ahead()))
    //     {
    //         if (t->precedence() < precedence)
    //             return left_operand;
    //         
    //         node = new ast::binary_operator();
    //         node->identifier(this->next_token_value());
    //         
    //         right_operand = this->parse_expression();
    //         
    //         if (this->look_ahead() != NULL)
    //         {   
    //             if (t->precedence() < this->look_ahead()->precedence())
    //                 right_operand = this->parse_binary_operator(t->precedence()+1, right_operand);
    //         }
    //         
    //         node->left_operand(left_operand);
    //         node->right_operand(right_operand);
    //         left_operand = node;
    //     }
    //     
    //     return left_operand;
    // }
}
