// chime: parser.h

#ifndef PARSER_H
#define PARSER_H

#include "compiler/lexer/lexer.h"
#include "ParseError.h"
#include "compiler/ast/node.h"
#include "compiler/ast/Root.h"
#include "compiler/ast/ScopedNode.h"

namespace chime
{
    class parser
    {
    public:
        parser(chime::lexer* lexer);
        virtual ~parser();
        
        TokenRef    nextToken(const char* expected=NULL);
        token*      next_token(void);
        token*      next_token(const char* expected);
        bool        advanceTokenIfEqual(const char* expected);
        bool        advance_token_if_equals(const char* expected);
        std::string nextTokenValue(const char* expected=NULL);
        std::string next_token_value(const char* expected=NULL);
        void        advanceToNextStatement();
        void        advance_past_ending_tokens(void);
        Token*      lookAhead(int distance=1);
        token*      look_ahead(void);
        token*      look_ahead(int);
        
        std::vector<chime::ParseErrorRef> getErrors() const;
        void                              addError(ParseErrorRef e);
        void                              addError(const char* message);
        void                              printErrors() const;
        
        ScopedNode* getCurrentScope() const;
        void        setCurrentScope(ScopedNode* node);
        void        pushScope(ScopedNode* node);
        void        popScope();
        
        Root*  parse(void);
        Node*  parse_without_structural(void);
        NodeRef parseExpression();
        Node*  parse_expression(void);
        Node*  parse_type(void);
        
        void        addSourceDependency(const std::string& dependency);
        void        addBinaryDependency(const std::string& dependency);
        
    protected:
        Root*                      _currentRoot;
        ScopedNode*                _currentScope;
        lexer*                     _lexer;
        std::vector<ParseErrorRef> _errors;
    };
    
    typedef std::tr1::shared_ptr<parser> ParserRef;
}

#endif // PARSER_H
