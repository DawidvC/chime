#include "parser_tests_base.h"

class LiteralParserTest : public ParserTestsBase
{
};

TEST_F(LiteralParserTest, String)
{
    ast::node* node;
    
    node = parse("\"a string\"");
    
    assert_literal("a string", node->child_at_index(0));
}

