#include "process.h"
#include "runtime/platform.h"

#ifdef PLATFORM_MAC_OS_X
#   include <dispatch/dispatch.h>
#endif

void chime_process_initialize(void)
{
    chime_class_t* klass;
    
    klass = chime_class_create_object_subclass("Process.Process");
    
    // instance methods
    chime_class_set_instance_method(klass, "run",    process_run);
}

chime_object_t* process_run(chime_object_t* instance)
{
#ifdef PLATFORM_MAC_OS_X
    dispatch_main();
#endif
    
    return CHIME_NULL;
}
