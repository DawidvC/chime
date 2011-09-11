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
    EXPECT_TRUE(token->is_string());
}

TEST_F(TokenTest, IsNumber)
{
    token->set_value("0");
    EXPECT_TRUE(token->is_number());
    token->set_value("1");
    EXPECT_TRUE(token->is_number());
    token->set_value("2");
    EXPECT_TRUE(token->is_number());
    token->set_value("3");
    EXPECT_TRUE(token->is_number());
    
    token->set_value("10");
    EXPECT_TRUE(token->is_number());
}

TEST_F(TokenTest, IsModifier)
{
    token->set_value("private");
    EXPECT_TRUE(token->is_modifier());
    
    token->set_value("protected");
    EXPECT_TRUE(token->is_modifier());
    
    token->set_value("abstract");
    EXPECT_TRUE(token->is_modifier());
    
    token->set_value("internal");
    EXPECT_TRUE(token->is_modifier());
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
    ASSERT_TRUE(token->is_type());
    
    token->set_value("ABCdef");
    ASSERT_TRUE(token->is_type());
    
    token->set_value("abc");
    ASSERT_TRUE(!token->is_type());
    
    token->set_value("aBC");
    ASSERT_TRUE(!token->is_type());
    
    token->set_value(".");
    ASSERT_TRUE(!token->is_type());
    
    token->set_value("\n");
    ASSERT_TRUE(!token->is_type());
    
    token->set_value("(");
    ASSERT_TRUE(!token->is_type());
    
    token->set_value("");
    ASSERT_TRUE(!token->is_type());
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
    ASSERT_TRUE(token->is_ending());
    
    token->set_value("\n");
    ASSERT_TRUE(token->is_ending());
    
    token->set_value("}");
    ASSERT_TRUE(token->is_ending());
}

TEST_F(TokenTest, IsControl)
{
    token->set_value("next");
    ASSERT_TRUE(token->is_control());
}

TEST_F(TokenTest, IsOperator)
{
    token->set_value(".");
    ASSERT_TRUE(token->precedence() > 0);
    
    token->set_value("<=>");
    ASSERT_TRUE(token->precedence() > 0);
}

TEST_F(TokenTest, IsLiteral)
{
    token->set_value("1");
    ASSERT_TRUE(token->is_literal());
    
    token->set_value("true");
    ASSERT_TRUE(token->is_literal());
    
    token->set_value("false");
    ASSERT_TRUE(token->is_literal());
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
