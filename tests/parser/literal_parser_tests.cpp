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

TEST_F(LiteralParserTest, Integer)
{
    ast::node* node;
    
    node = parse("128");
    
    assert_literal(128, node->child_at_index(0));
}

TEST_F(LiteralParserTest, True)
{
    ast::node* node;
    
    node = parse("true")->child_at_index(0);
    
    ASSERT_LITERAL_TRUE(node);
}

TEST_F(LiteralParserTest, False)
{
    ast::node* node;
    
    node = parse("false")->child_at_index(0);
    
    ASSERT_LITERAL_FALSE(node);
}
