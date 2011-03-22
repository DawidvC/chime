#include <gtest/gtest.h>
#include "../../frontend/chime.h"
#include "../../ast/ast.h"
#include "parse_helpers.h"

#ifndef PARSER_TEST_BASE
#define PARSER_TEST_BASE

class ParserTestsBase : public testing::Test
{
protected:
    virtual void SetUp();
    
    virtual void TearDown();
    
    ast::node* parse(const char *input);
    
    ast::implementation* parse_implemenation(const char *input);
    ast::method_definition* parse_method_def(const char *input);
    
protected:
    ast::node* _last_node;
};

#endif
