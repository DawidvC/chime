#include <string>
#include <vector>
#include <assert.h>

#include "parser.h"

#define LOG(x) do { printf(x); fflush(stdout); } while (0);

namespace chime
{
    parser::parser(chime::lexer* lexer)
    {
        _lexer = lexer;
        _lexer->ignore_new_lines(false);
        
        _errors = new std::vector<chime::parse_error*>();
    }
    
    parser::~parser()
    {
        delete _errors;
    }
    
#pragma mark *** Token Handling ***
    token* parser::next_token(void)
    {
        return this->next_token(NULL);
    }
    token* parser::next_token(const char* expected)
    {
        token* t = _lexer->next_token();
        
        if (t == NULL)
        {
            LOG("[Parser] Reached the end of the file unexpectedly\n")
            return NULL;
        }
        
        if ((expected != NULL) && (!t->equal_to(expected)))
        {
            chime::parse_error* e;
            
            e = new chime::parse_error("Expected '%s' but got '%s'", expected, t->value.c_str());
            
            this->errors()->push_back(e);
            
            return NULL;
        }
        
        return t;
    }
    bool parser::advance_token_if_equals(const char* expected)
    {
        token* t;
        
        t = this->look_ahead();
        if ((t != NULL) && t->equal_to(expected))
        {
            t = this->next_token(expected);
            delete t;
            return true;
        }
        
        return false;
    }
    std::string parser::next_token_value(void)
    {
        return this->next_token_value(NULL);
    }
    std::string parser::next_token_value(const char* expected)
    {
        token *t = this->next_token(expected);
        if (!t)
            return std::string();
        
        std::string v = t->value;
        
        delete t;
        
        return v;
    }
    void   parser::advance_past_ending_tokens(void)
    {
        token* t;
        
        while (true)
        {
            t = this->look_ahead();
            if (!t->is_ending())
                break;
                
            if (t->empty() || t->value == "}")
                break;
            
            if (t->value == "\n")
            {
                this->next_token_value("\n");
            }
            else if (t->value == ";")
            {
                this->next_token_value(";");
            }
        }
    }
    
    token* parser::look_ahead(void)
    {
        return this->look_ahead(1);
    }
    token* parser::look_ahead(int distance)
    {
        return _lexer->look_ahead(distance);
    }
    
    std::vector<chime::parse_error*>* parser::errors(void) const
    {
        return _errors;
    }
    
#pragma mark *** Parsing Methods ***
    ast::node* parser::parse(void)
    {
        ast::node* root;
        
        root = new ast::node();
        
        while (true)
        {
            ast::node* node;
            
            node = parse_with_structural();
            if (node == NULL)
                break;
            
            root->add_child(node);
        }
        
        return root;
    }
    
    ast::node* parser::parse_with_structural(void)
    {
        ast::node*    node;
        chime::token* t;
        
        t = this->look_ahead();
        if (t->empty())
            return NULL;
        
        node = NULL;
        
        if (t->equal_to("import"))
        {
            node = new ast::import(this);
        }
        else if (t->equal_to("implementation"))
        {
            node = new ast::implementation(this);
        }
        else if (t->equal_to("method"))
        {
            node = new ast::method_definition(this);
        }
        else
        {
            node = this->parse_without_structural();
        }
        
        if (!node)
        {
            chime::parse_error* e;
            
            e = new chime::parse_error("Unable to construct in the main loop");
            
            this->errors()->push_back(e);
        }
        
        return node;
    }
    
    ast::node* parser::parse_without_structural(void)
    {
        chime::token* t;
        ast::node*    node;
        
        node = NULL;
        
        t = this->look_ahead();
        if (t->empty())
        {
            chime::parse_error* e;
            
            e = new chime::parse_error("parse_without_structural: found empty/null token");
            
            this->errors()->push_back(e);
            return NULL;
        }
        else if (t->is_control())
        {
            chime::parse_error* e;
            
            e = new chime::parse_error("parse_without_structural: control not implemented");
            
            this->errors()->push_back(e);
            return NULL;
            // node = this->parse_parentheses();
        }
        else if (t->is_type())
        {
            node = this->parse_type();
            
            t = this->look_ahead();
            if (t->empty())
            {
                return node;
            }
            else if (t->is_identifier())
            {
                return new ast::variable_definition(this, (ast::type_reference*)node);
            }
            else if (t->precedence() > 0)
            {
                return this->parse_binary_operator(0, node);
            }
            else
            {
                chime::parse_error* e;

                e = new chime::parse_error("parse_without_structural: found something unexpected '%s'", t->value.c_str());

                this->errors()->push_back(e);
                return NULL;
            }
        }
        
        node = this->parse_expression();
        
        return node;
    }
    
    ast::node* parser::parse_expression(void)
    {
        chime::token* t;
        ast::node*    node;
        
        node = NULL;
        
        t = this->look_ahead();
        if (t == NULL || t->empty())
        {
            chime::parse_error* e;
            
            e = new chime::parse_error("Unable to extract the next token while making an expression");
            
            this->errors()->push_back(e);
            
            return NULL;
        }
        
        if (t->equal_to("("))
        {
            chime::parse_error* e;
            
            e = new chime::parse_error("expression: parentheses not implemented");
            
            this->errors()->push_back(e);
            
            return NULL;
            // node = this->parse_parentheses();
        }
        else if(t->is_literal())
        {
            chime::parse_error* e;
            
            e = new chime::parse_error("expression: literal not implemented");
            
            this->errors()->push_back(e);
            
            return NULL;
            // node = this->parse_literal();
        }
        else if (t->is_type())
        {
            node = this->parse_type();
        }
        else if (t->is_identifier())
        {
            if (this->look_ahead(2)->equal_to("("))
            {
                return new ast::method_call(this);
            }
            
            node = new ast::entity_reference(this);
        }
        else
        {
            chime::parse_error* e;
            
            e = new chime::parse_error("expression: found something weird '%s'", t->value.c_str());
            
            this->errors()->push_back(e);
            
            return NULL;
        }
        
        return this->parse_binary_operator(0, node);
    }
    
    ast::node* parser::parse_binary_operator(int precedence, ast::node* left_operand)
    {
        ast::binary_operator* node;
        ast::node*            right_operand;
        token*                t;
        int                   current_precedence;
        
        while (true)
        {
            t = this->look_ahead();
            if (t->empty())
                return left_operand;
            
            if (t->is_ending())
            {
                this->advance_past_ending_tokens();
                return left_operand;
            }
            
            // we have to store this, because t is just a look-ahead token,
            // and may be removed by future parsing operations
            current_precedence = t->precedence();
            
            if (current_precedence < precedence)
                return left_operand;
            
            node = new ast::binary_operator();
            node->identifier(this->next_token_value());
            
            right_operand = this->parse_expression();
            
            if (current_precedence < this->look_ahead()->precedence())
            {
                right_operand = this->parse_binary_operator(current_precedence+1, right_operand);
            }
            
            node->left_operand(left_operand);
            node->right_operand(right_operand);
            left_operand = node;
        }
        
        return left_operand;
    }
    
    ast::node* parser::parse_type(void)
    {
        chime::token* t;
        ast::node*    node;
        
        node = new ast::type_reference(this);
        
        t = this->look_ahead();
        
        // we might have an operator on a type
        if (t->precedence() > 0)
        {
            node = this->parse_binary_operator(0, node);
        }
        
        return node;
    }
}
