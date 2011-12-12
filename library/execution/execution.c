#include "execution.h"
#include "library/execution/queue_methods.h"

void chime_execution_initialize(void)
{
    chime_class_t* execution_context_class;
    chime_class_t* klass;
    
    execution_context_class = chime_class_create_object_subclass("Execution.Context");
    
    klass = chime_class_create("Execution.Queue", execution_context_class);
    
    // instance methods
    chime_class_set_instance_method(klass, "initialize", execution_queue_initialize);
    chime_class_set_instance_method(klass, "finalize",   execution_queue_finalize);
    chime_class_set_instance_method(klass, "sync:",      execution_queue_sync);
    chime_class_set_instance_method(klass, "async:",     execution_queue_async);
    chime_class_set_instance_method(klass, "wait",       execution_queue_wait);
}
