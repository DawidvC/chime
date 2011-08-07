#include "chime_array.h"
#include <assert.h>

void chime_array_initialize(void)
{
    chime_object_t* test_case_class;
    
    test_case_class = chime_runtime_create_object_subclass("Array");
    assert(test_case_class);
}
