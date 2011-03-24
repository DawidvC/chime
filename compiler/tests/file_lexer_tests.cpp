#include <gtest/gtest.h>
#include "../lexer/file_lexer.h"

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
        
        s += "/Users/matt/Documents/programming/chime/compiler/tests/files/";
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

#pragma mark token functions
TEST_F(FileLexerTest, BasicImplementation)
{
    lex("basic_implementation.chm");
    
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
