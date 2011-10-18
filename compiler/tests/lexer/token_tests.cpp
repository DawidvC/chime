#include "compiler/lexer/token.h"
#ifndef USING_PREFIX_HEADERS
#    include <gtest/gtest.h>
#endif

class TokenTest : public testing::Test
{
protected:
    chime::token *token;
    
    virtual void SetUp()
    {
        token = new chime::token();
    }
    virtual void TearDown()
    {
        delete token;
    }
};

TEST_F(TokenTest, Constructor)
{
    EXPECT_FALSE(token == NULL);
    EXPECT_EQ("", token->value());
}

TEST_F(TokenTest, Empty)
{
    ASSERT_TRUE(token->empty());
    
    token->clear_value();
    ASSERT_TRUE(token->empty());
    
    token->append((char)0);
    ASSERT_TRUE(token->empty());
}

TEST_F(TokenTest, IsEqual)
{
    token->set_value("abc123");
    ASSERT_TRUE(token->equal_to("abc123"));
    ASSERT_FALSE(token->equal_to("123abc"));
}

TEST_F(TokenTest, IsString)
{
    token->set_value("\"string\"");
    ASSERT_TRUE(token->isString());
    
    token->set_value("\"string\"{");
    ASSERT_TRUE(token->isString());
    
    token->set_value("}\"string\"");
    ASSERT_TRUE(token->isString());
}

TEST_F(TokenTest, IsInteger)
{
    token->set_value("1");
    ASSERT_TRUE(token->isInteger());
    
    token->set_value("123");
    ASSERT_TRUE(token->isInteger());
    
    token->set_value("0");
    ASSERT_TRUE(token->isInteger());
    
    token->set_value("1.5");
    ASSERT_FALSE(token->isInteger());
    
    token->set_value("-1");
    ASSERT_TRUE(token->isInteger());
    
    token->setValue("0x01");
    ASSERT_TRUE(token->isInteger());
    
    token->setValue("1i");
    ASSERT_TRUE(token->isInteger());
    
    token->setValue("1j");
    ASSERT_TRUE(token->isInteger());
    
    token->set_value("-");
    ASSERT_FALSE(token->isInteger());
    
    token->set_value("Abc");
    ASSERT_FALSE(token->isInteger());
    
    token->setValue("1j1");
    ASSERT_FALSE(token->isInteger());
    
    token->setValue("i");
    ASSERT_FALSE(token->isInteger());
    
    token->setValue("j");
    ASSERT_FALSE(token->isInteger());
}

TEST_F(TokenTest, IntegerValue)
{
    token->setValue("1");
    ASSERT_EQ(1, token->integerValue());
    
    token->setValue("0x01");
    ASSERT_EQ(1, token->integerValue());
}

TEST_F(TokenTest, IsFloatingPoint)
{
    token->set_value("1.0");
    ASSERT_TRUE(token->isFloatingPoint());
    
    token->set_value("0.5");
    ASSERT_TRUE(token->isFloatingPoint());
    
    token->set_value("-1.0");
    ASSERT_TRUE(token->isFloatingPoint());
    
    token->set_value("123");
    ASSERT_FALSE(token->isFloatingPoint());
    
    token->set_value("-");
    ASSERT_FALSE(token->isFloatingPoint());
    
    token->set_value("Abc");
    ASSERT_FALSE(token->isFloatingPoint());
}

TEST_F(TokenTest, IsModifier)
{
    token->set_value("private");
    EXPECT_TRUE(token->isModifier());
    
    token->set_value("protected");
    EXPECT_TRUE(token->isModifier());
    
    token->set_value("abstract");
    EXPECT_TRUE(token->isModifier());
    
    token->set_value("internal");
    EXPECT_TRUE(token->isModifier());
}

TEST_F(TokenTest, IsStructural)
{
    token->set_value("method");
    ASSERT_TRUE(token->isStructural());
    
    token->set_value("class");
    ASSERT_TRUE(token->isStructural());
    
    token->set_value("import");
    ASSERT_TRUE(token->isStructural());
    
    token->set_value("interface");
    ASSERT_TRUE(token->isStructural());
    
    token->set_value("implementation");
    ASSERT_TRUE(token->isStructural());
    
    token->set_value("attribute");
    ASSERT_TRUE(token->isStructural());
}

TEST_F(TokenTest, IsType)
{
    token->set_value("Abc");
    ASSERT_TRUE(token->isType());
    
    token->set_value("ABCdef");
    ASSERT_TRUE(token->isType());
    
    token->set_value("abc");
    ASSERT_TRUE(!token->isType());
    
    token->set_value("aBC");
    ASSERT_TRUE(!token->isType());
    
    token->set_value(".");
    ASSERT_TRUE(!token->isType());
    
    token->set_value("\n");
    ASSERT_TRUE(!token->isType());
    
    token->set_value("(");
    ASSERT_TRUE(!token->isType());
    
    token->set_value("");
    ASSERT_TRUE(!token->isType());
}

TEST_F(TokenTest, IsIdentifier)
{
    token->set_value("abc");
    ASSERT_TRUE(token->isIdentifier());
    
    token->set_value("Abc");
    ASSERT_TRUE(!token->isIdentifier());
    
    token->set_value(":");
    ASSERT_TRUE(!token->isIdentifier());
    
    token->set_value("");
    ASSERT_TRUE(!token->isIdentifier());
    
    token->set_value("do");
    ASSERT_TRUE(!token->isIdentifier());
}

TEST_F(TokenTest, IsEnding)
{
    token->set_value(";");
    ASSERT_TRUE(token->isStatementEnding());
    ASSERT_TRUE(!token->isBlockEnding());
    
    token->set_value("\n");
    ASSERT_TRUE(token->isStatementEnding());
    ASSERT_TRUE(!token->isBlockEnding());
    
    token->set_value("}");
    ASSERT_TRUE(token->isStatementEnding());
    ASSERT_TRUE(token->isBlockEnding());
}

TEST_F(TokenTest, IsControl)
{
    token->set_value("next");
    ASSERT_TRUE(token->isControl());
}

TEST_F(TokenTest, IsOperator)
{
    token->set_value(".");
    ASSERT_TRUE(token->precedence() > 0);
    
    token->setValue("+");
    ASSERT_TRUE(token->precedence() > 0);
    
    token->setValue("+=");
    ASSERT_TRUE(token->precedence() > 0);
    
    token->setValue("-");
    ASSERT_TRUE(token->precedence() > 0);
    
    token->setValue("-=");
    ASSERT_TRUE(token->precedence() > 0);
    
    token->setValue("*");
    ASSERT_TRUE(token->precedence() > 0);
    
    token->setValue("*=");
    ASSERT_TRUE(token->precedence() > 0);
    
    token->setValue("/");
    ASSERT_TRUE(token->precedence() > 0);
    
    token->setValue("/=");
    ASSERT_TRUE(token->precedence() > 0);
    
    token->setValue("<");
    ASSERT_TRUE(token->precedence() > 0);
    
    token->setValue("<=");
    ASSERT_TRUE(token->precedence() > 0);
    
    token->setValue(">");
    ASSERT_TRUE(token->precedence() > 0);
    
    token->setValue(">=");
    ASSERT_TRUE(token->precedence() > 0);
    
    token->set_value("<=>");
    ASSERT_TRUE(token->precedence() > 0);
    
    token->setValue("=");
    ASSERT_TRUE(token->precedence() > 0);
    
    token->setValue("==");
    ASSERT_TRUE(token->precedence() > 0);
    
    token->setValue("and");
    ASSERT_TRUE(token->precedence() > 0);
    
    token->setValue("or");
    ASSERT_TRUE(token->precedence() > 0);
}

TEST_F(TokenTest, IsLiteral)
{
    token->set_value("1");
    ASSERT_TRUE(token->isLiteral());
    
    token->set_value("true");
    ASSERT_TRUE(token->isLiteral());
    
    token->set_value("false");
    ASSERT_TRUE(token->isLiteral());
    
    token->set_value("nil");
    ASSERT_TRUE(token->isLiteral());
}

TEST_F(TokenTest, IsExceptionRelated)
{
    token->set_value("try");
    ASSERT_TRUE(token->isExceptionRelated());
    
    token->set_value("catch");
    ASSERT_TRUE(token->isExceptionRelated());
    
    token->set_value("finally");
    ASSERT_TRUE(token->isExceptionRelated());
    
    token->set_value("throw");
    ASSERT_TRUE(token->isExceptionRelated());
}
