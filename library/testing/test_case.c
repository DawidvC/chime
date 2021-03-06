// Chime Runtime: test_case.c

#include "library/testing/test_case.h"

#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

static chime_object_t* test_case_setup(chime_object_t* instance);
static chime_object_t* test_case_teardown(chime_object_t* instance);

void chime_test_case_initialize(void)
{
    chime_class_t* test_case_class;
    
    test_case_class = chime_class_create_object_subclass("Test.Case");
    
    chime_class_set_instance_method(test_case_class, "setup",    test_case_setup);
    chime_class_set_instance_method(test_case_class, "teardown", test_case_teardown);
}

// object functions
chime_object_t* test_case_setup(chime_object_t* instance)
{
    return CHIME_LITERAL_NULL;
}

chime_object_t* test_case_teardown(chime_object_t* instance)
{
    return CHIME_LITERAL_NULL;
}