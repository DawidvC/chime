#include <gtest/gtest.h>
#include "runtime/chime_runtime.h"
#include "runtime/string/chime_string.h"
#include "runtime/hash/chime_hash.h"

class RuntimeHashTests : public testing::Test
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

TEST_F(RuntimeHashTests, CreateAndSet)
{
    chime_object_t* hash;
    chime_object_t* key;
    chime_object_t* value;
    
    hash  = chime_hash_create();
    key   = chime_string_create_with_c_string("test", 4);
    value = chime_object_create_with_name("Object");
    
    chime_hash_set(hash, key, value);
    
    // this works because the key is the same object
    ASSERT_TRUE(chime_object_invoke_1(hash, "[]", key) == value);
    
    key = chime_string_create_with_c_string("test", 4);
    
    ASSERT_TRUE(chime_object_invoke_1(hash, "[]", key) == value);
}
