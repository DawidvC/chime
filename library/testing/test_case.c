// Chime Runtime: test_case.c

#include "library/testing/test_case.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/chime_literals.h"
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

void chime_test_case_initialize(void)
{
    chime_object_t* test_case_class;
    
    test_case_class = chime_runtime_create_class("Test.Case", _object_class);
    assert(test_case_class);
}
