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
    fprintf(stderr, "invoked!?\n");
    
    return 0;
}

TEST_F(RuntimeTests, InvokeMethod)
{
    chime_object_t* object;
    
    object = chime_object_create_with_name("Object");
    
    chime_object_set_function(object, "test_method", RuntimeTestsFunctionNoArgs);
    
    chime_object_invoke(object, "test_method", 5);
    
    chime_object_destroy(object);
}