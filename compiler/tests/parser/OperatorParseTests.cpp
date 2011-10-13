#include "parser_tests_base.h"

class OperatorParseTests : public ParserTestsBase
{
};

TEST_F(OperatorParseTests, AdditionAndAssignment)
{
    chime::BinaryOperator* op;
    
    op = parseFirst<chime::BinaryOperator*>("a = a + 1");
    
    ASSERT_GLOBAL_ASSIGNMENT(op);
    ASSERT_GLOBAL_VARIABLE("a", op->getLeftOperand());
    
    op = static_cast<chime::BinaryOperator*>(op->getRightOperand());
    ASSERT_GLOBAL_VARIABLE("a", op->getLeftOperand());
    ASSERT_LITERAL_INTEGER(1, op->getRightOperand());
}

TEST_F(OperatorParseTests, CompoundAdditionAndAssignment)
{
    chime::BinaryOperator* op;
    
    op = parseFirst<chime::BinaryOperator*>("a += 1");
    
    ASSERT_GLOBAL_ASSIGNMENT(op);
    ASSERT_GLOBAL_VARIABLE("a", op->getLeftOperand());
    
    op = static_cast<chime::BinaryOperator*>(op->getRightOperand());
    ASSERT_GLOBAL_VARIABLE("a", op->getLeftOperand());
    ASSERT_LITERAL_INTEGER(1, op->getRightOperand());
}

TEST_F(OperatorParseTests, IndexerCall)
{
    ast::IndexOperator* op;
    
    op = parseFirst<ast::IndexOperator*>("foo[123]");
    
    ASSERT_INDEXER(op);
    ASSERT_LITERAL_INTEGER(123, op->getArgument().get());
    ASSERT_GLOBAL_VARIABLE("foo", op->getOperand().get());
}

TEST_F(OperatorParseTests, IndexerOnMethod)
{
    ast::BinaryOperator* op;
    ast::IndexOperator*  indexer;
    
    indexer = parseFirst<ast::IndexOperator*>("foo.bar[123]");
    
    indexer->print();
    
    ASSERT_INDEXER(indexer);
    ASSERT_LITERAL_INTEGER(123, indexer->getArgument().get());
    
    op = static_cast<ast::BinaryOperator*>(indexer->getOperand().get());
    
    ASSERT_OPERATOR(".", op);
    ASSERT_GLOBAL_VARIABLE("foo", op->getLeftOperand());
    ASSERT_METHOD_CALL("bar", op->getRightOperand());
}
