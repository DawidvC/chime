// Chime Runtime: chime_runtime.c

#include "library/chime_library.h"
#include "runtime/chime_runtime.h"
#include "library/execution/execution.h"
#include "library/data/chime_data.h"
#include "library/network/network.h"
#include "library/process/process.h"
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
    
    chime_execution_initialize();
    
    chime_network_initialize();
    
    chime_process_initialize();
    
    chime_data_initialize();
    
    chime_log_level = old_level; // restore logging
}

void chime_library_destroy(void)
{
}
