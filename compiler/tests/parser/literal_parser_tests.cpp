#include "parser_tests_base.h"

class LiteralParserTest : public ParserTestsBase
{
};

TEST_F(LiteralParserTest, String)
{
    ast::node* node;
    
    node = parse("\"a string\"");
    
    ASSERT_LITERAL_STRING("a string", node->childAtIndex(0));
}

TEST_F(LiteralParserTest, Integer)
{
    ast::node* node;
    
    node = parse("128");
    
    ASSERT_LITERAL_INTEGER(128, node->childAtIndex(0));
}

TEST_F(LiteralParserTest, NegativeInteger)
{
    ast::node* node;
    
    node = parse("-128");
    
    ASSERT_LITERAL_INTEGER(-128, node->childAtIndex(0));
}

TEST_F(LiteralParserTest, True)
{
    ast::node* node;
    
    node = parse("true")->childAtIndex(0);
    
    ASSERT_LITERAL_TRUE(node);
}

TEST_F(LiteralParserTest, False)
{
    ast::node* node;
    
    node = parse("false")->childAtIndex(0);
    
    ASSERT_LITERAL_FALSE(node);
}

TEST_F(LiteralParserTest, Self)
{
    ast::node* node;
    
    node = parse("self")->childAtIndex(0);
    
    ASSERT_LITERAL_SELF(node);
}

TEST_F(LiteralParserTest, StringWithIndexer)
{
    ast::IndexOperator* op;
    
    op = static_cast<ast::IndexOperator*>(parse("\"a string\"[123]")->childAtIndex(0));
    
    ASSERT_INDEXER(op);
    ASSERT_LITERAL_STRING("a string", op->getOperand().get());
    ASSERT_LITERAL_INTEGER(123, op->getArgument().get());
}

TEST_F(LiteralParserTest, SimpleInterpolatedString)
{
    ast::binary_operator* op;
    
    // translates to: "a string" + (1 + 1).toString() + ""
    op = static_cast<ast::binary_operator*>(parse("\"1 + 2 = {1 + 2}\"")->childAtIndex(0));
    
    ASSERT_OPERATOR("+", op);
    ASSERT_LITERAL_STRING("", op->getRightOperand());
    
    op = static_cast<ast::binary_operator*>(op->getLeftOperand());
    ASSERT_OPERATOR("+", op);
    ASSERT_LITERAL_STRING("1 + 2 = ", op->getLeftOperand());
    
    op = static_cast<ast::binary_operator*>(op->getRightOperand());
    ASSERT_OPERATOR(".", op);
    ASSERT_METHOD_CALL("to_string", op->getRightOperand());
    
    op = static_cast<ast::binary_operator*>(op->getLeftOperand());
    ASSERT_OPERATOR("+", op);
    ASSERT_LITERAL_INTEGER(1, op->getLeftOperand());
    ASSERT_LITERAL_INTEGER(2, op->getRightOperand());
}