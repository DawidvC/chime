#include <gtest/gtest.h>
#include "runtime/chime_runtime.h"
#include "runtime/core/object/chime_object.h"
#include "runtime/classes/integer/chime_integer.h"

class MemoryManagementTests : public testing::Test
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

TEST_F(MemoryManagementTests, CreateAndDestroy)
{
    chime_object_t* obj;
    
    obj = chime_object_create_with_name("Object");
    
    ASSERT_EQ(1, chime_object_get_retain_count(obj));
    
    chime_object_release(obj);
}

TEST_F(MemoryManagementTests, RetainsBalancedByReleases)
{
    chime_object_t* obj;
    
    obj = chime_object_create_with_name("Object"); // +1
    
    chime_object_retain(obj); // +2
    chime_object_retain(obj); // +3
    
    chime_object_release(obj); // +2
    chime_object_release(obj); // +1
    chime_object_release(obj); // 0
}

TEST_F(MemoryManagementTests, RetainAndReleaseSafeOnIntegers)
{
    chime_object_t* obj;
    
    obj = chime_integer_encode(1); // +1
    
    chime_object_retain(obj); // +2 (for regular objects)
    
    chime_object_release(obj); // +1
    chime_object_release(obj); // 0 (destroy for regular objects)
    chime_object_release(obj); // overrelease!
    chime_object_release(obj); // for good measure, overrelease again
}
