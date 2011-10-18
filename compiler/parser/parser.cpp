#include "parser.h"
#include "compiler/ast/ast.h"
#include "compiler/ast/control/Control.h"
#include "compiler/ast/literals/literal.h"
#include "compiler/ast/structural/Structural.h"

#include <string>
#include <vector>
#include <assert.h>

namespace chime
{
    parser::parser(chime::lexer* lexer)
    {
        _lexer = lexer;
        _lexer->ignore_new_lines(false);
        
        _currentRoot  = NULL;
        _currentScope = NULL;
    }
    
    parser::~parser()
    {
    }
    
#pragma mark *** Token Handling ***
    TokenRef parser::nextToken(const char* expected)
    {
        TokenRef token;
        
        token = TokenRef(_lexer->next_token());
        
        if (!token)
        {
            this->addError("Reached the end of the file unexpectedly");
            return token;
        }
        
        if ((expected != NULL) && (!token->equal_to(expected)))
        {
            chime::ParseErrorRef e;
            
            e = chime::ParseErrorRef(new chime::ParseError("Expected '%s' but got '%s'", expected, token->c_str()));
            
            this->addError(e);
            
            return token;
        }
        
        return token;
    }
    
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
            chime::ParseErrorRef e;
            
            e = chime::ParseErrorRef(new chime::ParseError("Expected '%s' but got '%s'", expected, t->c_str()));
            
            this->addError(e);
            
            return NULL;
        }
        
        return t;
    }
    bool parser::advanceTokenIfEqual(const char* expected)
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
    
    bool parser::advance_token_if_equals(const char* expected)
    {
        return this->advanceTokenIfEqual(expected);
    }
    std::string parser::nextTokenValue(const char* expected)
    {
        token *t = this->next_token(expected);
        
        if (!t)
            return std::string();
        
        std::string v = t->value();
        
        delete t;
        
        return v;
    }
    std::string parser::next_token_value(const char* expected)
    {
        return this->nextTokenValue(expected);
    }
    void parser::advanceToNextStatement()
    {
        token* t;
        
        while (true)
        {
            t = this->look_ahead();
            if (!t->isStatementEnding())
                break;
                
            if (t->empty() || t->isBlockEnding())
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
    void parser::advance_past_ending_tokens(void)
    {
        this->advanceToNextStatement();
    }
    
    Token* parser::lookAhead(int distance)
    {
        return _lexer->look_ahead(distance);
    }
    token* parser::look_ahead(void)
    {
        return this->lookAhead();
    }
    token* parser::look_ahead(int distance)
    {
        return this->lookAhead(distance);
    }
    
    std::vector<chime::ParseErrorRef> parser::getErrors() const
    {
        return _errors;
    }
    
    void parser::addError(chime::ParseErrorRef e)
    {
        e->line(_lexer->current_line());
        
        _errors.push_back(e);
    }
    
    void parser::addError(const char* message)
    {
        chime::ParseErrorRef e;
        
        e = chime::ParseErrorRef(new chime::ParseError(message));
        
        this->addError(e);
    }
    
    void parser::printErrors(void) const
    {
        std::vector<chime::ParseErrorRef>::const_iterator it;
        
        for (it = _errors.begin(); it != _errors.end(); ++it)
        {
            fprintf(stdout, "[Parse:\e[31merror\e[0m:%d] %s\n", (*it)->line(), (*it)->message().c_str());
        }
    }

#pragma mark -
#pragma mark Context Handling
    ast::ScopedNode* parser::getCurrentScope() const
    {
        return _currentScope;
    }
    
    void parser::setCurrentScope(ast::ScopedNode* scope)
    {
        _currentScope = scope;
    }
    
    void parser::pushScope(ast::ScopedNode* scope)
    {
        if (!_currentScope)
        {
            _currentScope = scope;
            return;
        }
        
        scope->setParent(_currentScope);
        
        _currentScope = scope;
    }
    
    void parser::popScope()
    {
        assert(_currentScope);
        assert(_currentScope->getParent());
        
        _currentScope = _currentScope->getParent();
    }
    
#pragma mark -
#pragma mark Parsing Methods
    ast::Root* parser::parse(void)
    {
        _currentRoot = new ast::Root();
        
        // this is the "global" scope, though it's really only global
        // to the current file
        this->pushScope(_currentRoot);
        
        while (true)
        {
            ast::node* node;
            
            this->advance_past_ending_tokens();
            
            node = ast::Structural::parse(*this);
            if (node == NULL)
                break;
            
            _currentRoot->addChild(node);
        }
        
        return _currentRoot;
    }
    
    ast::node* parser::parse_without_structural(void)
    {
        chime::token* t;
        ast::node*    node;
        
        node = NULL;
        
        t = this->look_ahead();
        if (t->empty())
        {
            this->addError("parse_without_structural: found empty/null token");
            return NULL;
        }
        else if (t->isControl())
        {
            return Control::parse(*this);
        }
        else if (t->isType())
        {
            node = this->parse_type();
            
            t = this->look_ahead();
            if (t->empty() || t->isStatementEnding())
            {
                return node;
            }
            else if (t->isIdentifier())
            {
                return new ast::variable_definition(this, (ast::type_reference*)node);
            }
            else if (t->precedence() > 0)
            {
                return ast::binary_operator::parse(*this, 0, node);
            }
            else
            {
                chime::ParseErrorRef e;
                
                e = ParseErrorRef(new chime::ParseError("parse_without_structural: found something unexpected '%s'", t->c_str()));
                
                this->addError(e);
                return NULL;
            }
        }
        
        node = this->parse_expression();
        
        node = Conditional::parseTailing(*this, node);
        
        return node;
    }
    
    ast::NodeRef parser::parseExpression()
    {
        return ast::NodeRef(this->parse_expression());
    }
    
    ast::node* parser::parse_expression()
    {
        chime::token* t;
        ast::node*    node;
        
        node = NULL;
        
        t = this->look_ahead();
        if (t == NULL || t->empty())
        {
            this->addError("Unable to extract the next token while making an expression");
            
            return NULL;
        }
        
        if (t->equal_to("("))
        {
            this->next_token_value("(");
            
            node = this->parse_expression();
            
            this->next_token_value(")");
        }
        else if (t->isLiteral())
        {
            node = ast::Literal::parse(*this);
        }
        else if (t->value() =="do")
        {
            return ast::Closure::parse(*this);
        }
        else if (t->isType())
        {
            node = this->parse_type();
        }
        else if (t->isIdentifier())
        {
            if (this->look_ahead(2)->equal_to("("))
            {
                return ast::MethodCall::parse(*this);
            }
            
            node = ast::Variable::parse(*this);
        }
        else
        {
            chime::ParseErrorRef e;
            
            fprintf(stderr, "Found something weird '%s'\n", t->c_str());
            
            __builtin_trap();
            
            e = chime::ParseErrorRef(new chime::ParseError("expression: found something weird '%s'", t->c_str()));
            
            this->addError(e);
            
            return NULL;
        }
        
        if (node && this->look_ahead()->isStatementEnding())
        {
            return node;
        }
        
        // be careful not to auto-advance to the next statement here,
        // as some semantics depend on knowing there's nothing following
        // the expression on the same line (like a tailing if)
        
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
            
            node = Conditional::parseTailing(*this, node);
        }
        
        return node;
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
