#include <gtest/gtest.h>
#include "runtime/chime_runtime.h"

class RuntimeLiteralTests : public testing::Test
{
protected:
    virtual void SetUp()
    {
        chime_runtime_initialize();
        chime_log_level = 3;
    }
    virtual void TearDown()
    {
        chime_runtime_destroy();
    }
};

TEST_F(RuntimeLiteralTests, Null)
{
    chime_object_t* object;
    
    object = CHIME_LITERAL_NULL;
    
    ASSERT_TRUE(object == 0);
}

TEST_F(RuntimeLiteralTests, Integer)
{
    chime_object_t* object;
    
    object = chime_literal_encode_integer(121);
    
    ASSERT_EQ(121, chime_literal_decode_integer(object));
    
    object = chime_literal_encode_integer(-121);
    
    ASSERT_EQ(-121, chime_literal_decode_integer(object));
    
    object = chime_literal_encode_integer(0);
    
    ASSERT_EQ(0, chime_literal_decode_integer(object));
}

TEST_F(RuntimeLiteralTests, MethodInvokedOnInteger)
{
    chime_object_t* a;
    
    a = chime_literal_encode_integer(1);
    
    chime_object_invoke(a, "a_method");
}

TEST_F(RuntimeLiteralTests, MethodInvokedOnIntegerWithArgument)
{
    chime_object_t* a;
    
    a = chime_literal_encode_integer(1);
    
    chime_object_invoke(a, "a_method", chime_literal_encode_integer(5));
}
