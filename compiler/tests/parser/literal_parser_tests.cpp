#include "parser_tests_base.h"

class LiteralParserTest : public ParserTestsBase
{
};

TEST_F(LiteralParserTest, String)
{
    chime::Node* node;
    
    node = parse("\"a string\"");
    
    ASSERT_LITERAL_STRING("a string", node->childAtIndex(0));
}

TEST_F(LiteralParserTest, Integer)
{
    chime::Node* node;
    
    node = parse("128");
    
    ASSERT_LITERAL_INTEGER(128, node->childAtIndex(0));
}

TEST_F(LiteralParserTest, FloatingPoint)
{
    chime::Node* node;
    
    node = parse("1.0");
    
    ASSERT_LITERAL_FLOAT(1.0, node->childAtIndex(0));
}

TEST_F(LiteralParserTest, NegativeInteger)
{
    chime::Node* node;
    
    node = parse("-128");
    
    ASSERT_LITERAL_INTEGER(-128, node->childAtIndex(0));
}

TEST_F(LiteralParserTest, HexadecimalInteger)
{
    chime::Node* node;
    
    node = parse("0x0000000A");
    
    ASSERT_LITERAL_INTEGER(10, node->childAtIndex(0));
}

TEST_F(LiteralParserTest, ImaginaryInteger)
{
    chime::Node* node;
    
    node = parse("7j");
    
    ASSERT_LITERAL_IMAGINARY_INTEGER(7, node->childAtIndex(0));
}

TEST_F(LiteralParserTest, True)
{
    chime::Node* node;
    
    node = parse("true")->childAtIndex(0);
    
    ASSERT_LITERAL_TRUE(node);
}

TEST_F(LiteralParserTest, False)
{
    chime::Node* node;
    
    node = parse("false")->childAtIndex(0);
    
    ASSERT_LITERAL_FALSE(node);
}

TEST_F(LiteralParserTest, Self)
{
    chime::Node* node;
    
    node = parse("self")->childAtIndex(0);
    
    ASSERT_LITERAL_SELF(node);
}

TEST_F(LiteralParserTest, StringWithIndexer)
{
    chime::IndexOperator* op;
    
    op = static_cast<chime::IndexOperator*>(parse("\"a string\"[123]")->childAtIndex(0));
    
    ASSERT_INDEXER(op);
    ASSERT_LITERAL_STRING("a string", op->getOperand().get());
    ASSERT_LITERAL_INTEGER(123, op->getArgument().get());
}

TEST_F(LiteralParserTest, SimpleInterpolatedString)
{
    chime::BinaryOperator* op;
    
    // translates to: "a string" + (1 + 1) + ""
    op = parseOperator("\"1 + 2 = {1 + 2}\"");
    
    ASSERT_OPERATOR("+", op);
    ASSERT_LITERAL_STRING("", op->getRightOperand());
    
    op = static_cast<chime::BinaryOperator*>(op->getLeftOperand());
    ASSERT_OPERATOR("+", op);
    ASSERT_LITERAL_STRING("1 + 2 = ", op->getLeftOperand());
    
    op = static_cast<chime::BinaryOperator*>(op->getRightOperand());
    ASSERT_OPERATOR("+", op);
    ASSERT_LITERAL_INTEGER(1, op->getLeftOperand());
    ASSERT_LITERAL_INTEGER(2, op->getRightOperand());
}

TEST_F(LiteralParserTest, LeadingInterpolationInString)
{
    chime::BinaryOperator* op;
    
    op = parseOperator("\"{1} + 1 = 2\"");
    
    ASSERT_OPERATOR("+", op);
    ASSERT_LITERAL_STRING(" + 1 = 2", op->getRightOperand());
}

TEST_F(LiteralParserTest, MultipleInterpolationString)
{
    chime::BinaryOperator* leftOp;
    chime::BinaryOperator* rightOp;
    
    leftOp = parseOperator("\" {1} {2} {3}\"");
    
    ASSERT_OPERATOR("+", leftOp);
    
    rightOp = static_cast<chime::BinaryOperator*>(leftOp->getRightOperand());
    leftOp = static_cast<chime::BinaryOperator*>(leftOp->getLeftOperand());
    
    ASSERT_OPERATOR("+", leftOp);
    ASSERT_LITERAL_STRING(" ", leftOp->getLeftOperand());
    ASSERT_LITERAL_INTEGER(1, leftOp->getRightOperand());
    
    ASSERT_OPERATOR("+", rightOp);
    
    leftOp  = static_cast<chime::BinaryOperator*>(rightOp->getLeftOperand());
    rightOp = static_cast<chime::BinaryOperator*>(rightOp->getRightOperand());
    
    ASSERT_OPERATOR("+", leftOp);
    ASSERT_LITERAL_STRING(" ", leftOp->getLeftOperand());
    ASSERT_LITERAL_INTEGER(2, leftOp->getRightOperand());
    
    ASSERT_OPERATOR("+", rightOp);
    ASSERT_LITERAL_STRING("", rightOp->getRightOperand());
    
    leftOp = static_cast<chime::BinaryOperator*>(rightOp->getLeftOperand());
    ASSERT_OPERATOR("+", leftOp);
    ASSERT_LITERAL_STRING(" ", leftOp->getLeftOperand());
    ASSERT_LITERAL_INTEGER(3, leftOp->getRightOperand());
}

TEST_F(LiteralParserTest, RangeLiteral)
{
    chime::RangeLiteral* range;
    
    range = parseFirst<chime::RangeLiteral*>("5:6");
    
    ASSERT_LITERAL_RANGE(5, 6, range);
}

TEST_F(LiteralParserTest, NilLiteral)
{
    chime::NilLiteral* nil;
    
    nil = parseFirst<chime::NilLiteral*>("nil");
    
    ASSERT_LITERAL_NIL(nil);
}

TEST_F(LiteralParserTest, ArrayLiteral)
{
    chime::ArrayLiteral* array;
    
    array = parseFirst<chime::ArrayLiteral*>("[1,2,3]");
    
    ASSERT_LITERAL_ARRAY(array);
    ASSERT_LITERAL_INTEGER(1, array->childAtIndex(0));
    ASSERT_LITERAL_INTEGER(2, array->childAtIndex(1));
    ASSERT_LITERAL_INTEGER(3, array->childAtIndex(2));
}

TEST_F(LiteralParserTest, HashLiteral)
{
    chime::HashLiteral* hash;
    
    hash = parseFirst<chime::HashLiteral*>("{\"a\":1,\"b\":2,\"c\":3}");
    
    ASSERT_LITERAL_HASH(hash);
    ASSERT_LITERAL_STRING( "a", hash->childAtIndex(0));
    ASSERT_LITERAL_INTEGER(  1, hash->childAtIndex(1));
    ASSERT_LITERAL_STRING( "b", hash->childAtIndex(2));
    ASSERT_LITERAL_INTEGER(  2, hash->childAtIndex(3));
    ASSERT_LITERAL_STRING( "c", hash->childAtIndex(4));
    ASSERT_LITERAL_INTEGER(  3, hash->childAtIndex(5));
}
