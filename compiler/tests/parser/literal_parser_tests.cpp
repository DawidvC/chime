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
    ast::BinaryOperator* op;
    
    // translates to: "a string" + (1 + 1) + ""
    op = parseOperator("\"1 + 2 = {1 + 2}\"");
    
    ASSERT_OPERATOR("+", op);
    ASSERT_LITERAL_STRING("", op->getRightOperand());
    
    op = static_cast<ast::BinaryOperator*>(op->getLeftOperand());
    ASSERT_OPERATOR("+", op);
    ASSERT_LITERAL_STRING("1 + 2 = ", op->getLeftOperand());
    
    op = static_cast<ast::BinaryOperator*>(op->getRightOperand());
    ASSERT_OPERATOR("+", op);
    ASSERT_LITERAL_INTEGER(1, op->getLeftOperand());
    ASSERT_LITERAL_INTEGER(2, op->getRightOperand());
}

TEST_F(LiteralParserTest, LeadingInterpolationInString)
{
    ast::BinaryOperator* op;
    
    op = parseOperator("\"{1} + 1 = 2\"");
    
    ASSERT_OPERATOR("+", op);
    ASSERT_LITERAL_STRING(" + 1 = 2", op->getRightOperand());
}

TEST_F(LiteralParserTest, MultipleInterpolationString)
{
    ast::BinaryOperator* leftOp;
    ast::BinaryOperator* rightOp;
    
    leftOp = parseOperator("\" {1} {2} {3}\"");
    
    ASSERT_OPERATOR("+", leftOp);
    
    rightOp = static_cast<ast::BinaryOperator*>(leftOp->getRightOperand());
    leftOp = static_cast<ast::BinaryOperator*>(leftOp->getLeftOperand());
    
    ASSERT_OPERATOR("+", leftOp);
    ASSERT_LITERAL_STRING(" ", leftOp->getLeftOperand());
    ASSERT_LITERAL_INTEGER(1, leftOp->getRightOperand());
    
    ASSERT_OPERATOR("+", rightOp);
    
    leftOp  = static_cast<ast::BinaryOperator*>(rightOp->getLeftOperand());
    rightOp = static_cast<ast::BinaryOperator*>(rightOp->getRightOperand());
    
    ASSERT_OPERATOR("+", leftOp);
    ASSERT_LITERAL_STRING(" ", leftOp->getLeftOperand());
    ASSERT_LITERAL_INTEGER(2, leftOp->getRightOperand());
    
    ASSERT_OPERATOR("+", rightOp);
    ASSERT_LITERAL_STRING("", rightOp->getRightOperand());
    
    leftOp = static_cast<ast::BinaryOperator*>(rightOp->getLeftOperand());
    ASSERT_OPERATOR("+", leftOp);
    ASSERT_LITERAL_STRING(" ", leftOp->getLeftOperand());
    ASSERT_LITERAL_INTEGER(3, leftOp->getRightOperand());
}
