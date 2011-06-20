// parser.h

#ifndef PARSER_H
#define PARSER_H

#include "lexer/lexer.h"
#include "parse_error.h"
#include "ast/ast.h"

namespace chime
{
    class parser
    {
    public:
        parser(chime::lexer* lexer);
        virtual ~parser();
        
        token*      next_token(void);
        token*      next_token(const char* expected);
        bool        advance_token_if_equals(const char* expected);
        std::string next_token_value(void);
        std::string next_token_value(const char* expected);
        void        advance_past_ending_tokens(void);
        token*      look_ahead(void);
        token*      look_ahead(int);
        
        std::vector<chime::parse_error*>* errors(void) const;
        void                              add_error(chime::parse_error* e);
        void                              addError(const char* message);
        void                              print_errors(void) const;
        
        ast::node*  parse(void);
        ast::node*  parse_with_structural(void);
        ast::node*  parse_without_structural(void);
        ast::node*  parse_primary(void);
        ast::node*  parse_expression(void);
        ast::node*  parse_binary_operator(int precedence, ast::node* left_operand);
        ast::node*  parse_type(void);
        ast::node*  parse_literal(void);
        ast::node*  parse_control(void);
        ast::node*  parse_tailing_conditional(ast::node* body_node);
        
    protected:
        chime::lexer*                     _lexer;
        std::vector<chime::parse_error*>* _errors;
    };
}

#endif // PARSER_H
