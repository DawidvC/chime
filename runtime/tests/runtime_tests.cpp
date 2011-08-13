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

TEST_F(RuntimeTests, GetNullType)
{
    chime_object_t* object;
    
    object = CHIME_LITERAL_NULL;
    
    ASSERT_EQ(CHIME_NULL_TYPE, chime_object_get_type(object));
    
    chime_object_destroy(object);
}

static chime_object_t* RuntimeTestsFunctionNoArgs(chime_object_t* self)
{
    return CHIME_LITERAL_NULL;
}

static chime_object_t* RuntimeTestsFunctionOneArg(chime_object_t* self, chime_object_t* arg1)
{
    return arg1;
}

TEST_F(RuntimeTests, InvokeMethod)
{
    chime_object_t* object;
    chime_object_t* objectClass;
    
    objectClass = chime_runtime_get_class("Object");
    
    chime_object_set_function(objectClass, "test_method", (void*)RuntimeTestsFunctionNoArgs, 0);
    
    object = chime_object_create(objectClass);
    
    ASSERT_TRUE(chime_object_invoke_0(object, "test_method") == CHIME_LITERAL_NULL);
    
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
    
    chime_object_set_function(objectClass, "test_method", (void*)RuntimeTestsFunctionOneArg, 1);
    
    ASSERT_TRUE(chime_object_invoke_1(object, "test_method", argument) == argument);
    
    chime_object_destroy(object);
}

TEST_F(RuntimeTests, InheritanceStructure)
{
    chime_object_t* object;
    chime_object_t* objectClass;
    chime_object_t* subobject;
    chime_object_t* subobjectClass;
    
    objectClass = chime_runtime_get_class("Object");
    object      = chime_object_create(objectClass);
    
    // make sure the object class is the one just created
    ASSERT_TRUE(chime_object_get_class(object) == objectClass);
    
    // the Object class has a metaclass
    ASSERT_TRUE(chime_object_get_class(objectClass) != 0);
    
    // the Object class has no superclass
    ASSERT_TRUE(chime_object_get_superclass(objectClass) == 0);
    
    subobjectClass = chime_runtime_create_class("SubObject", objectClass);
    
    // the subobjectClass's superclass should be the objectClass
    ASSERT_TRUE(chime_object_get_superclass(subobjectClass) == objectClass);
    
    // the superclass of the metaclass is the metaclass of the superclass
    // (say that fives times fast)
    ASSERT_TRUE(chime_object_get_superclass(chime_object_get_class(subobjectClass)) == chime_object_get_class(objectClass));
    
    subobject = chime_object_create(subobjectClass);
    
    // make sure the type is right
    ASSERT_TRUE(chime_object_get_class(subobject) == subobjectClass);
    
    // make sure the super is Object
    ASSERT_TRUE(chime_object_get_superclass(subobject) == objectClass);
}

TEST_F(RuntimeTests, OverwriteProperty)
{
    chime_object_t* object;
    chime_object_t* test_prop;
    
    object    = chime_object_create_with_name("Object");
    test_prop = chime_object_create_with_name("Object");
    
    chime_object_set_property(object, "test_prop", test_prop);
    
    ASSERT_EQ(test_prop, chime_object_get_property(object, "test_prop"));
    
    // now overwrite it with something
    chime_object_set_property(object, "test_prop", CHIME_LITERAL_TRUE);
    
    ASSERT_EQ(CHIME_LITERAL_TRUE, chime_object_get_property(object, "test_prop"));
    
    chime_object_destroy(object);
    chime_object_destroy(test_prop);
}
