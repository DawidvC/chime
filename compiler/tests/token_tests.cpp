#include <gtest/gtest.h>
#include "../lexer/token.h"

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
    EXPECT_EQ("", token->value);
}

TEST_F(TokenTest, Empty)
{
    EXPECT_TRUE(token->empty());
}

TEST_F(TokenTest, IsEqual)
{
    token->value = "abc123";
    ASSERT_TRUE(token->equal_to("abc123"));
    ASSERT_FALSE(token->equal_to("123abc"));
}

TEST_F(TokenTest, IsString)
{
    token->value = "\"string\"";
    EXPECT_TRUE(token->is_string());
}

TEST_F(TokenTest, IsNumber)
{
    token->value = "0";
    EXPECT_TRUE(token->is_number());
    token->value = "1";
    EXPECT_TRUE(token->is_number());
    token->value = "2";
    EXPECT_TRUE(token->is_number());
    token->value = "3";
    EXPECT_TRUE(token->is_number());
    
    token->value = "10";
    EXPECT_TRUE(token->is_number());
}

TEST_F(TokenTest, IsModifier)
{
    token->value = "private";
    EXPECT_TRUE(token->is_modifier());
    
    token->value = "protected";
    EXPECT_TRUE(token->is_modifier());
    
    token->value = "abstract";
    EXPECT_TRUE(token->is_modifier());
    
    token->value = "internal";
    EXPECT_TRUE(token->is_modifier());
}

TEST_F(TokenTest, IsStructural)
{
    token->value = "method";
    ASSERT_TRUE(token->is_structural());
    
    token->value = "class";
    ASSERT_TRUE(token->is_structural());
    
    token->value = "namespace";
    ASSERT_TRUE(token->is_structural());
    
    token->value = "import";
    ASSERT_TRUE(token->is_structural());
    
    token->value = "interface";
    ASSERT_TRUE(token->is_structural());
    
    token->value = "implementation";
    ASSERT_TRUE(token->is_structural());
}

TEST_F(TokenTest, IsType)
{
    token->value = "Abc";
    ASSERT_TRUE(token->is_type());
    
    token->value = "ABCdef";
    ASSERT_TRUE(token->is_type());
    
    token->value = "abc";
    ASSERT_TRUE(!token->is_type());
    
    token->value = "aBC";
    ASSERT_TRUE(!token->is_type());
    
    token->value = ".";
    ASSERT_TRUE(!token->is_type());
}