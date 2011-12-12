// Chime Library: execution/queue_methods.h

#ifndef EXECUTION_QUEUE_METHODS_H
#define EXECUTION_QUEUE_METHODS_H

#include "runtime/chime_runtime.h"

#ifdef __cplusplus
extern "C" {
#endif

chime_object_t* execution_queue_initialize(chime_object_t* instance);
chime_object_t* execution_queue_finalize(chime_object_t* instance);

chime_object_t* execution_queue_sync(chime_object_t* instance, chime_object_t* function);
chime_object_t* execution_queue_async(chime_object_t* instance, chime_object_t* function);
chime_object_t* execution_queue_wait(chime_object_t* instance);

#ifdef __cplusplus
}
#endif

#endif // EXECUTION_QUEUE_METHODS_H
