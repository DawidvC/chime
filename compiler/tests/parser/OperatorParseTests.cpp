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
