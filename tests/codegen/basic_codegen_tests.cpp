#include <gtest/gtest.h>
#include "codegen/code_generator.h"

class CodeGenTests : public testing::Test
{
protected:
    virtual void SetUp()
    {
    }
    virtual void TearDown()
    {
    }
};

TEST_F(CodeGenTests, CreateCodeGenerator)
{
    chime::code_generator* generator;
    
    generator = new chime::code_generator();
    
    EXPECT_FALSE(generator == NULL);
    
    delete generator;
}