#include "serial_queue.h"
#include "execution.h"
#include "runtime/classes/string/chime_string.h"
#include "runtime/tagging.h"

chime_object_t* execution_serial_queue_new(chime_object_t* instance, chime_object_t* name)
{
    chime_object_t* object;
    
    object = chime_object_create(chime_runtime_get_class("Execution.Queue"));
    
    chime_object_invoke_1(object, "initialize:", name);
    
    return object;
}

chime_object_t* execution_serial_queue_initialize(chime_object_t* instance, chime_object_t* name)
{
    chime_object_set_attribute(instance, "name", name);
    
#ifdef PLATFORM_MAC_OS_X
    dispatch_queue_t queue;
    
    queue = chime_allocate(1);
    chime_zero_memory(queue, 1);
    
    chime_object_set_attribute(instance, "_dispatch_objects", chime_tag_encode_raw_block(queue));
    
    execution_context_set_dispatch_queue(instance, dispatch_queue_create(chime_string_to_c_string(name), 0));
    execution_context_set_dispatch_group(instance, dispatch_group_create());
#endif
    
    return instance;
}

chime_object_t* execution_serial_queue_finalize(chime_object_t* instance)
{
#ifdef PLATFORM_MAC_OS_X
    dispatch_queue_t queue;
    
    queue = execution_context_get_dispatch_queue(instance);
    dispatch_release(queue);
#endif
    
    return NULL;
}
