#include <gtest/gtest.h>
#include "runtime/chime_runtime.h"
#include "runtime/core/class/chime_class.h"
#include "runtime/core/object/chime_object.h"

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
    chime_class_t*  object_class;
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
    chime_class_t*  object_class;
    
    object_class = chime_runtime_get_class("Object");
    
    chime_class_set_instance_method(object_class, "test_method", (void*)RuntimeTestsFunctionNoArgs);
    
    object = chime_object_create(object_class);
    
    ASSERT_TRUE(chime_object_invoke_0(object, "test_method") == CHIME_LITERAL_NULL);
    
    chime_object_destroy(object);
}

TEST_F(RuntimeTests, InvokeMethodWithOneArg)
{
    chime_object_t* object;
    chime_class_t*  object_class;
    chime_object_t* argument;
    
    object_class = chime_runtime_get_class("Object");
    
    object   = chime_object_create(object_class);
    argument = chime_object_create(object_class);
    
    chime_class_set_instance_method(object_class, "test_method", (void*)RuntimeTestsFunctionOneArg);
    
    ASSERT_TRUE(chime_object_invoke_1(object, "test_method", argument) == argument);
    
    chime_object_destroy(argument);
    chime_object_destroy(object);
}

TEST_F(RuntimeTests, InheritanceStructure)
{
    chime_object_t* object;
    chime_class_t*  object_class;
    chime_object_t* subobject;
    chime_class_t*  subobject_class;
    
    object_class = chime_runtime_get_class("Object");
    object       = chime_object_create(object_class);
    
    // make sure the object class is the one just created
    ASSERT_TRUE((chime_class_t*)chime_object_get_class(object) == object_class);
    
    // the Object class has a metaclass
    ASSERT_TRUE(chime_class_get_class(object_class) != 0);
    
    // the Object class has no superclass
    ASSERT_TRUE(chime_class_get_superclass(object_class) == 0);
    
    subobject_class = chime_class_create("SubObject", object_class);
    
    // the subobjectClass's superclass should be the objectClass
    ASSERT_TRUE(chime_class_get_superclass(subobject_class) == object_class);
    
    // the superclass of the metaclass is the metaclass of the superclass
    // (say that fives times fast)
    ASSERT_TRUE(chime_class_get_superclass(chime_class_get_class(subobject_class)) == chime_class_get_class(object_class));
    
    subobject = chime_object_create(subobject_class);
    
    // make sure the type is right
    ASSERT_TRUE((chime_class_t*)chime_object_get_class(subobject) == subobject_class);
    
    // make sure the super is Object
    ASSERT_TRUE((chime_class_t*)chime_object_get_superclass(subobject) == object_class);
    
    chime_object_destroy(subobject);
    chime_object_destroy(object);
}

TEST_F(RuntimeTests, OverwriteOneAttribute)
{
    chime_object_t* object;
    chime_object_t* test_prop;
    
    object    = chime_object_create_with_name("Object");
    test_prop = chime_object_create_with_name("Object");
    
    chime_object_set_attribute(object, "test_prop", test_prop);
    
    ASSERT_EQ(test_prop, chime_object_get_attribute(object, "test_prop"));
    
    // now overwrite it with something
    chime_object_set_attribute(object, "test_prop", CHIME_LITERAL_TRUE);
    
    ASSERT_EQ(CHIME_LITERAL_TRUE, chime_object_get_attribute(object, "test_prop"));
    
    // try a third time
    chime_object_set_attribute(object, "test_prop", test_prop);
    
    ASSERT_EQ(test_prop, chime_object_get_attribute(object, "test_prop"));
    
    chime_object_destroy(object);
    chime_object_destroy(test_prop);
}

TEST_F(RuntimeTests, OverwriteMiddleAttribute)
{
    chime_object_t* object;
    chime_object_t* test_prop;
    
    object    = chime_object_create_with_name("Object");
    test_prop = chime_object_create_with_name("Object");
    
    chime_object_set_attribute(object, "one", CHIME_LITERAL_TRUE);
    chime_object_set_attribute(object, "two", CHIME_LITERAL_TRUE);
    chime_object_set_attribute(object, "three", CHIME_LITERAL_TRUE);
    
    ASSERT_EQ(CHIME_LITERAL_TRUE, chime_object_get_attribute(object, "two"));
    
    chime_object_set_attribute(object, "two", test_prop);
    
    ASSERT_EQ(CHIME_LITERAL_TRUE, chime_object_get_attribute(object, "one"));
    ASSERT_EQ(test_prop, chime_object_get_attribute(object, "two"));
    ASSERT_EQ(CHIME_LITERAL_TRUE, chime_object_get_attribute(object, "three"));
    
    chime_object_destroy(object);
    chime_object_destroy(test_prop);
}

TEST_F(RuntimeTests, OverwriteLastAttribute)
{
    chime_object_t* object;
    chime_object_t* test_prop;
    
    object    = chime_object_create_with_name("Object");
    test_prop = chime_object_create_with_name("Object");
    
    chime_object_set_attribute(object, "one", CHIME_LITERAL_TRUE);
    chime_object_set_attribute(object, "two", CHIME_LITERAL_TRUE);
    chime_object_set_attribute(object, "three", CHIME_LITERAL_TRUE);
    
    ASSERT_EQ(CHIME_LITERAL_TRUE, chime_object_get_attribute(object, "three"));
    
    chime_object_set_attribute(object, "three", test_prop);
    
    ASSERT_EQ(CHIME_LITERAL_TRUE, chime_object_get_attribute(object, "one"));
    ASSERT_EQ(CHIME_LITERAL_TRUE, chime_object_get_attribute(object, "two"));
    ASSERT_EQ(test_prop, chime_object_get_attribute(object, "three"));
    
    chime_object_destroy(object);
    chime_object_destroy(test_prop);
}

TEST_F(RuntimeTests, NullAttribute)
{
    chime_object_t* object;
    
    object = chime_object_create_with_name("Object");
    
    chime_object_set_attribute(object, "one", NULL);
    
    ASSERT_TRUE(chime_object_get_attribute(object, "one") == NULL);
}
