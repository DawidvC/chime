#include "queue_methods.h"
#include "runtime/core/closure/chime_closure.h"
#include "runtime/platform.h"
#include "runtime/tagging.h"

#include <stdio.h>

#ifdef PLATFORM_MAC_OS_X
#   include <dispatch/dispatch.h>
#endif

#ifdef PLATFORM_MAC_OS_X
dispatch_queue_t execution_queue_get_dispatch_queue(chime_object_t* instance)
{
    dispatch_queue_t* data;
    
    data = chime_tag_decode_raw_block(chime_object_get_attribute(instance, "_dispatch_queue"));
    
    return data[0];
}

void execution_queue_set_dispatch_queue(chime_object_t* instance, dispatch_queue_t queue)
{
    dispatch_queue_t* data;
    
    data = (dispatch_queue_t*)chime_tag_decode_raw_block(chime_object_get_attribute(instance, "_dispatch_queue"));
    
    data[0] = queue;
}
#endif

chime_object_t* execution_queue_initialize(chime_object_t* instance)
{
#ifdef PLATFORM_MAC_OS_X
    execution_queue_set_dispatch_queue(instance, dispatch_queue_create("org.chimelang.queue", 0));
#endif

    return instance;
}

chime_object_t* execution_queue_finalize(chime_object_t* instance)
{
#ifdef PLATFORM_MAC_OS_X
    dispatch_queue_t queue;
    
    queue = execution_queue_get_dispatch_queue(instance);
    dispatch_release(queue);
#endif
    
    fprintf(stderr, "finalized\n");
    
    return NULL;
}

chime_object_t* execution_queue_sync(chime_object_t* instance, chime_object_t* function)
{
#ifdef PLATFORM_MAC_OS_X
    dispatch_sync(execution_queue_get_dispatch_queue(instance), ^{
        chime_closure_invoke((chime_closure_t*)function, NULL);
    });
#endif
    
    return NULL;
}

chime_object_t* execution_queue_async(chime_object_t* instance, chime_object_t* function)
{
#ifdef PLATFORM_MAC_OS_X
    dispatch_async(execution_queue_get_dispatch_queue(instance), ^{
        chime_closure_invoke((chime_closure_t*)function, NULL);
    });
#endif
    
    return NULL;
}

chime_object_t* execution_queue_wait(chime_object_t* instance)
{
#ifdef PLATFORM_MAC_OS_X
    dispatch_sync(execution_queue_get_dispatch_queue(instance), ^{});
#endif
    
    return NULL;
}
