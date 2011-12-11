#include <gtest/gtest.h>
#include "runtime/chime_runtime.h"
#include "runtime/classes/integer/chime_integer.h"
#include "runtime/core/object/chime_object.h"

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

TEST_F(RuntimeLiteralTests, Integer)
{
    chime_object_t* object;
    
    object = chime_integer_encode(121);
    
    ASSERT_EQ(121, chime_integer_decode(object));
    
    object = chime_integer_encode(-121);
    
    ASSERT_EQ(-121, chime_integer_decode(object));
    
    object = chime_integer_encode(0);
    
    ASSERT_EQ(0, chime_integer_decode(object));
}

TEST_F(RuntimeLiteralTests, MethodInvokedOnInteger)
{
    chime_object_t* a;
    
    a = chime_integer_encode(1);
    
    chime_object_invoke_0(a, "a_method");
}

TEST_F(RuntimeLiteralTests, MethodInvokedOnIntegerWithArgument)
{
    chime_object_t* a;
    
    a = chime_integer_encode(1);
    
    chime_object_invoke_1(a, "a_method", chime_integer_encode(5));
}
