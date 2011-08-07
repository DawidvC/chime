#ifndef PARSER_TEST_BASE
#define PARSER_TEST_BASE

#include "compiler/lexer/lexer.h"
#include "compiler/ast/ast.h"
#include "compiler/tests/parser/parse_helpers.h"

#include <gtest/gtest.h>

class ParserTestsBase : public testing::Test
{
protected:
    virtual void SetUp();
    
    virtual void TearDown();
    
    virtual chime::lexer* lex(const char* input);
    ast::node*            parse(const char *input);
    
    ast::Implementation*    parse_implementation(const char *input);
    ast::method_definition* parse_method_def(const char *input);
    ast::method_call*       parse_method_call(const char* input);
    
protected:
    ast::node* _last_node;
};

#endif