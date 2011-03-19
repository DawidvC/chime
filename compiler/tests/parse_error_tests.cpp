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
    chime::parse_error error("yo %d", 1);
    
    fprintf(stderr, "string %s", error.message().c_str());
}
