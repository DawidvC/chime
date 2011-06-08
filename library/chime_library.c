// Chime Runtime: chime_runtime.c

#include "library/chime_library.h"
#include "runtime/chime_runtime.h"
#include "library/testing/assertions.h"
#include "library/testing/test_case.h"
#include <stdio.h>

void chime_library_initialize(void)
{
    unsigned char old_level;
    
    old_level = chime_log_level;
    chime_log_level = 3;
    
    chime_assertion_initialize();
    chime_test_case_initialize();
    
    chime_log_level = old_level; // restore logging
}

void chime_library_destroy(void)
{
}