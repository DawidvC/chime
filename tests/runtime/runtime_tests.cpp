#include <gtest/gtest.h>
#include "runtime/chime_runtime.h"

class RuntimeTests : public testing::Test
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

TEST_F(RuntimeTests, GetObjectType)
{
    chime_object_t* object;
    
    object = chime_object_create_with_name("Object");
    
    ASSERT_EQ(CHIME_OBJECT_TYPE, chime_object_get_type(object));
    
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
    chime_object_t* objectClass;
    
    objectClass = chime_runtime_get_class("Object");
    
    chime_object_set_function(objectClass, "test_method", RuntimeTestsFunctionNoArgs, 0);
    
    object = chime_object_create(objectClass);
    
    ASSERT_TRUE(chime_object_invoke(object, "test_method") == CHIME_LITERAL_NULL);
    
    chime_object_destroy(object);
}

TEST_F(RuntimeTests, InvokeMethodWithOneArg)
{
    chime_object_t* object;
    chime_object_t* objectClass;
    chime_object_t* argument;
    
    objectClass = chime_runtime_get_class("Object");
    
    object   = chime_object_create(objectClass);
    argument = chime_object_create(objectClass);
    
    chime_object_set_function(objectClass, "test_method", RuntimeTestsFunctionOneArg, 1);
    
    ASSERT_TRUE(chime_object_invoke(object, "test_method", argument) == argument);
    
    chime_object_destroy(object);
}
