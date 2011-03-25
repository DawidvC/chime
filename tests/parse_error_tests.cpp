#include <gtest/gtest.h>
#include "../parser/parse_error.h"

class ParseErrorTest : public testing::Test
{
protected:
    virtual void SetUp()
    {
    }
    virtual void TearDown()
    {
    }
};

TEST_F(ParseErrorTest, Constructor)
{
    chime::parse_error* error;
    
    error = new chime::parse_error("yo %d", 1);
    
    ASSERT_EQ("yo 1", error->message());
    
    delete error;
}
