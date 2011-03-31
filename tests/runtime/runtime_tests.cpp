#include <gtest/gtest.h>
#include "runtime/chime_runtime.h"

class RuntimeTests : public testing::Test
{
protected:
    virtual void SetUp()
    {
        chime_runtime_initialize();
    }
    virtual void TearDown()
    {
        chime_runtime_destroy();
    }
};

TEST_F(RuntimeTests, InitializeRuntime)
{
    chime_object_t* object_class;
    chime_object_t* object;
    
    object_class = chime_runtime_get_class("Object");
    
    ASSERT_TRUE(object_class != NULL);
    
    object = chime_object_create(object_class);
    
    ASSERT_TRUE(object != NULL);
    
    chime_object_destroy(object);
}

static chime_object_t* RuntimeTestsFunctionNoArgs(chime_object_t* self, const char* method_name, ...)
{
    return CHIME_LITERAL_NULL;
}

static chime_object_t* RuntimeTestsFunctionOneArg(chime_object_t* self, const char* method_name, ...)
{
    va_list         arguments;
    chime_object_t* object;
    
    va_start(arguments, method_name);
    
    object = va_arg(arguments, chime_object_t*);
    
    va_end(arguments);
    
    return object;
}

TEST_F(RuntimeTests, InvokeMethod)
{
    chime_object_t* object;
    
    object   = chime_object_create_with_name("Object");
    
    chime_object_set_function(object, "test_method", RuntimeTestsFunctionNoArgs, 0);
    
    ASSERT_TRUE(chime_object_invoke(object, "test_method") == CHIME_LITERAL_NULL);
    
    chime_object_destroy(object);
}

TEST_F(RuntimeTests, InvokeMethodWithOneArg)
{
    chime_object_t* object;
    chime_object_t* argument;
    
    object   = chime_object_create_with_name("Object");
    argument = chime_object_create_with_name("Object");
    
    chime_object_set_function(object, "test_method", RuntimeTestsFunctionOneArg, 1);
    
    ASSERT_TRUE(chime_object_invoke(object, "test_method", argument) == argument);
    
    chime_object_destroy(object);
}
