#include "execution.h"
#include "library/execution/serial_queue.h"
#include "library/execution/concurrent_queue.h"
#include "runtime/core/closure/chime_closure.h"
#include "runtime/classes/integer/chime_integer.h"
#include "runtime/tagging.h"

#include <assert.h>

#ifdef PLATFORM_UNIX
#   include <unistd.h>
#endif

void chime_execution_initialize(void)
{
    chime_class_t* execution_context_class;
    chime_class_t* klass;
    
    execution_context_class = chime_class_create_object_subclass("Execution.Context");
    
    // class methods
    chime_class_set_class_method(execution_context_class, "sleep:", execution_context_sleep);
    
    // instance methods
    chime_class_set_instance_method(execution_context_class, "sync:",  execution_context_sync);
    chime_class_set_instance_method(execution_context_class, "async:", execution_context_async);
    chime_class_set_instance_method(execution_context_class, "wait",   execution_context_wait);
    
    klass = chime_class_create("Execution.Queue", execution_context_class);
    
    // class methods
    chime_class_set_class_method(klass, "new:", execution_serial_queue_new);
    
    // instance methods
    chime_class_set_instance_method(klass, "initialize:", execution_serial_queue_initialize);
    chime_class_set_instance_method(klass, "finalize",    execution_serial_queue_finalize);
    
    klass = chime_class_create("Execution.ConcurrentQueue", execution_context_class);
    
    // class methods
    chime_class_set_class_method(klass, "new:",    execution_concurrent_queue_new);
    chime_class_set_class_method(klass, "default", execution_concurrent_queue_get_default);
    
    // instance methods
    chime_class_set_instance_method(klass, "initialize:", execution_concurrent_queue_initialize);
    chime_class_set_instance_method(klass, "finalize",    execution_concurrent_queue_finalize);
}

chime_object_t* execution_context_sleep(chime_object_t* instance, chime_object_t* duration)
{
#ifdef PLATFORM_UNIX
    usleep(chime_integer_decode(duration));
#endif
    
    return CHIME_NULL;
}

#ifdef PLATFORM_MAC_OS_X
dispatch_queue_t execution_context_get_dispatch_queue(chime_object_t* instance)
{
    char** data;
    
    data = (char**)chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_dispatch_objects"));
    
    assert(data[0]);
    
    return (dispatch_queue_t)data[0];
}

void execution_context_set_dispatch_queue(chime_object_t* instance, dispatch_queue_t queue)
{
    char** data;
    
    assert(queue);
    
    data = (char**)chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_dispatch_objects"));
    
    data[0] = (char*)queue;
}

dispatch_group_t execution_context_get_dispatch_group(chime_object_t* instance)
{
    char** data;
    
    data = (char**)chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_dispatch_objects"));
    
    assert(data[1]);
    
    return (dispatch_group_t)data[1];
}

void execution_context_set_dispatch_group(chime_object_t* instance, dispatch_group_t group)
{
    char** data;
    
    assert(group);
    
    data = (char**)chime_tag_decode_raw_block(chime_object_get_attribute_unretained(instance, "_dispatch_objects"));
    
    data[1] = (char*)group;
}
#endif

chime_object_t* execution_context_sync(chime_object_t* instance, chime_object_t* function)
{
#ifdef PLATFORM_MAC_OS_X
    chime_object_retain(function);
    dispatch_sync(execution_context_get_dispatch_queue(instance), ^{
        chime_closure_invoke_0((chime_closure_t*)function);
        chime_object_release(function);
    });
#endif
    
    return NULL;
}

chime_object_t* execution_context_async(chime_object_t* instance, chime_object_t* function)
{
#ifdef PLATFORM_MAC_OS_X
    dispatch_queue_t queue;
    dispatch_group_t group;
    
    queue = execution_context_get_dispatch_queue(instance);
    group = execution_context_get_dispatch_group(instance);
    
    chime_object_retain(function);
    dispatch_group_async(group, queue, ^{
        chime_closure_invoke_0((chime_closure_t*)function);
        chime_object_release(function);
    });
#endif
    
    return NULL;
}

chime_object_t* execution_context_wait(chime_object_t* instance)
{
#ifdef PLATFORM_MAC_OS_X
    dispatch_group_t group;
    
    group = execution_context_get_dispatch_group(instance);
    dispatch_group_wait(group, DISPATCH_TIME_FOREVER);
#endif
    
    return NULL;
}
