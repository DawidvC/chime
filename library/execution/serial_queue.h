// Chime Library: execution/serial_queue.h

#ifndef EXECUTION_SERIAL_QUEUE_METHODS_H
#define EXECUTION_SERIAL_QUEUE_METHODS_H

#include "runtime/chime_runtime.h"

#ifdef __cplusplus
extern "C" {
#endif

chime_object_t* execution_serial_queue_new(chime_object_t* instance, chime_object_t* name);

chime_object_t* execution_serial_queue_initialize(chime_object_t* instance, chime_object_t* name);
chime_object_t* execution_serial_queue_finalize(chime_object_t* instance);

#ifdef __cplusplus
}
#endif

#endif // EXECUTION_SERIAL_QUEUE_METHODS_H
