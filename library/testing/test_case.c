// Chime Runtime: test_case.c

#include "library/testing/test_case.h"
#include "runtime/chime_literals.h"
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

static chime_object_t* test_case_setup(chime_object_t* instance, const char* method_name, ...);
static chime_object_t* test_case_teardown(chime_object_t* instance, const char* method_name, ...);

void chime_test_case_initialize(void)
{
    chime_object_t* test_case_class;
    
    test_case_class = chime_runtime_create_object_subclass("Test.Case");
    assert(test_case_class);
    
    chime_object_set_function(test_case_class, "setup",    test_case_setup,    0);
    chime_object_set_function(test_case_class, "teardown", test_case_teardown, 0);
}

// object functions
static chime_object_t* test_case_setup(chime_object_t* instance, const char* method_name, ...)
{
    return CHIME_LITERAL_NULL;
}

static chime_object_t* test_case_teardown(chime_object_t* instance, const char* method_name, ...)
{
    return CHIME_LITERAL_NULL;
}