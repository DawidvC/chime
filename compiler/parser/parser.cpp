#include "parser.h"
#include "compiler/ast/ast.h"

#include <string>
#include <vector>
#include <assert.h>

namespace chime
{
    parser::parser(chime::lexer* lexer)
    {
        _lexer = lexer;
        _lexer->ignore_new_lines(false);
        
        _errors = new std::vector<chime::parse_error*>();
        
        _currentRoot = NULL;
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
            this->addError("Reached the end of the file unexpectedly");
            return NULL;
        }
        
        if ((expected != NULL) && (!t->equal_to(expected)))
        {
            chime::parse_error* e;
            
            e = new chime::parse_error("Expected '%s' but got '%s'", expected, t->c_str());
            
            this->add_error(e);
            
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
        
        std::string v = t->value();
        
        delete t;
        
        return v;
    }
    void parser::advance_past_ending_tokens(void)
    {
        token* t;
        
        while (true)
        {
            t = this->look_ahead();
            if (!t->is_ending())
                break;
                
            if (t->empty() || t->equal_to("}"))
                break;
            
            if (t->equal_to("\n"))
            {
                this->next_token_value("\n");
            }
            else if (t->equal_to(";"))
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
    
    void parser::add_error(chime::parse_error* e)
    {
        assert(e != NULL);
        
        e->line(_lexer->current_line());
        
        this->errors()->push_back(e);
    }
    
    void parser::addError(const char* message)
    {
        chime::parse_error* e;
        
        e = new chime::parse_error(message);
        
        this->add_error(e);
    }
    
    void parser::print_errors(void) const
    {
        std::vector<chime::parse_error*>::iterator i;
        
        if (this->errors()->empty())
            return;
        
        for (i=this->errors()->begin(); i < this->errors()->end(); i++)
        {
            chime::parse_error* e;
            
            e = (*i);
            
            fprintf(stdout, "[Parse:\e[31merror\e[0m:%d] %s\n", e->line(), e->message().c_str());
        }
    }
    
#pragma mark *** Parsing Methods ***
    ast::Root* parser::parse(void)
    {
        _currentRoot = new ast::Root();
        
        while (true)
        {
            ast::node* node;
            
            this->advance_past_ending_tokens();
            
            node = parse_with_structural();
            if (node == NULL)
                break;
            
            _currentRoot->add_child(node);
        }
        
        return _currentRoot;
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
            node = new ast::Import(*this);
        }
        else if (t->equal_to("implementation"))
        {
            node = new ast::Implementation(*this);
        }
        else if (t->equal_to("method"))
        {
            node = new ast::method_definition(this);
        }
        else if (t->equal_to("property"))
        {
            node = new ast::PropertyDefinition(*this);
        }
        else
        {
            node = this->parse_without_structural();
        }
        
        if (!node)
        {
            chime::parse_error* e;
            
            e = new chime::parse_error("Unable to construct in the main loop");
            
            this->add_error(e);
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
            
            this->add_error(e);
            return NULL;
        }
        else if (t->is_control())
        {
            return this->parse_control();
        }
        else if (t->is_type())
        {
            node = this->parse_type();
            
            t = this->look_ahead();
            if (t->empty() || t->is_ending())
            {
                return node;
            }
            else if (t->is_identifier())
            {
                return new ast::variable_definition(this, (ast::type_reference*)node);
            }
            else if (t->precedence() > 0)
            {
                return ast::binary_operator::parse(*this, 0, node);
            }
            else
            {
                chime::parse_error* e;
                
                e = new chime::parse_error("parse_without_structural: found something unexpected '%s'", t->c_str());
                
                this->add_error(e);
                return NULL;
            }
        }
        
        node = this->parse_expression();
        
        node = this->parse_tailing_conditional(node);
        
        return node;
    }
    
    ast::node* parser::parse_primary(void)
    {
        chime::token* t;
        ast::node*    node;
        
        node = NULL;
        
        t = this->look_ahead();
        if (t == NULL || t->empty())
        {
            chime::parse_error* e;
            
            e = new chime::parse_error("Unable to extract the next token while parsing a primary expression");
            
            this->add_error(e);
            
            return NULL;
        }
        else if (t->equal_to("("))
        {
            this->next_token_value("(");
            
            node = this->parse_expression();
            
            this->next_token_value(")");
        }
        else if (t->is_literal())
        {
            node = this->parse_literal();
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
            assert(0 && "how did we get to this point");
        }
        
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
            
            this->add_error(e);
            
            return NULL;
        }
        
        if (t->equal_to("("))
        {
            this->next_token_value("(");
            
            node = this->parse_expression();
            
            this->next_token_value(")");
        }
        else if (t->is_literal())
        {
            node = this->parse_literal();
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
            
            e = new chime::parse_error("expression: found something weird '%s'", t->c_str());
            
            this->add_error(e);
            
            return NULL;
        }
        
        return ast::binary_operator::parse(*this, 0, node);
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
            node = ast::binary_operator::parse(*this, 0, node);
        }
        
        return node;
    }
    
    ast::node* parser::parse_literal(void)
    {
        chime::token* t;
        ast::node*    node;
        
        t = this->look_ahead();
        if (t->is_floating_point())
        {
            assert(false);
        }
        else if (t->is_integer())
        {
            node = new ast::integer_literal(this);
        }
        else if (t->is_string())
        {
            node = new ast::string_literal(this);
        }
        else if (t->is_boolean())
        {
            node = new ast::boolean_literal(this);
        }
        else
        {
            assert(0 && "Unhandled literal!");
        }
        
        return node;
    }
    
    ast::node* parser::parse_control(void)
    {
        chime::token* t;
        ast::node*    node;
        
        t = this->look_ahead();
        if (t->equal_to("next"))
        {
            node = new ast::next(this);
            
            node = this->parse_tailing_conditional(node);
        }
        else if (t->equal_to("try"))
        {
            node = new ast::Try(*this);
        }
        else if (t->equal_to("throw"))
        {
            node = new ast::Throw(*this);
            
            node = this->parse_tailing_conditional(node);
        }
        else if (t->equal_to("if"))
        {
            node = new ast::IfStatement(*this, NULL);
        }
        else if (t->equal_to("return"))
        {
            node = new ast::Return(*this);
        }
        else
        {
            assert(false && "Unhandled control statement");
        }
        
        return node;
    }
    
    ast::node* parser::parse_tailing_conditional(ast::node* body_node)
    {
        chime::token* t;
        ast::node*    node;
        
        t = this->look_ahead();
        if (!t->is_conditional())
        {
            return body_node;
        }
        else if (t->equal_to("if"))
        {
            node = new ast::IfStatement(*this, body_node);
            
            this->advance_past_ending_tokens();
            return node;
        }
        
        return body_node;
    }
    
    void parser::addSourceDependency(const std::string& dependency)
    {
        assert(_currentRoot);
        
        _currentRoot->addSourceDependency(dependency);
    }
    
    void parser::addBinaryDependency(const std::string& dependency)
    {
        assert(_currentRoot);
        
        _currentRoot->addBinaryDependency(dependency);
    }
}