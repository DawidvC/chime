#include "compiler/lexer/file_lexer.h"
#ifndef USING_PREFIX_HEADERS
#    include <gtest/gtest.h>
#endif

class FileLexerTest : public testing::Test
{
protected:
    chime::filelexer* _lexer;
    
    virtual void SetUp()
    {
        _lexer = NULL;
    }
    
    virtual void TearDown()
    {
        delete _lexer;
    }
    
    void lex(const char *input)
    {
        std::string s;
        
        s += "/tmp/chime/compiler/tests/fixtures/";
        s += input;
        
        _lexer = new chime::filelexer(s.c_str());
    }
    
    chime::token* next_token(void)
    {
        return _lexer->next_token();
    }
    std::string next_token_value(void)
    {
        return this->next_token()->value();
    }
};

// *** token functions
TEST_F(FileLexerTest, BasicImplementation)
{
    lex("basic_implementation.cm");
    
    ASSERT_EQ("implementation", next_token_value());
    ASSERT_EQ("Foo",            next_token_value());
    ASSERT_EQ(":",              next_token_value());
    ASSERT_EQ("Bar",            next_token_value());
    ASSERT_EQ("{",              next_token_value());
    ASSERT_EQ("method",         next_token_value());
    ASSERT_EQ("something",      next_token_value());
    ASSERT_EQ("(",              next_token_value());
    ASSERT_EQ(")",              next_token_value());
    ASSERT_EQ("{",              next_token_value());
    ASSERT_EQ("}",              next_token_value());
    ASSERT_EQ("}",              next_token_value());
    
    ASSERT_TRUE(next_token()->empty());
}

TEST_F(FileLexerTest, StringWithCarriageReturn)
{
    lex("string_with_carriage_return.cm");
    
    ASSERT_EQ("a",           next_token_value());
    ASSERT_EQ("=",           next_token_value());
    ASSERT_EQ("\"hello\n\"",   next_token_value());
    
    ASSERT_TRUE(next_token()->empty());
}
