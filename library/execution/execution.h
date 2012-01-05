// Chime Library: execution/context.h

#ifndef EXECUTION_CONTEXT_H
#define EXECUTION_CONTEXT_H

#include "runtime/platform.h"

#ifdef PLATFORM_MAC_OS_X
#   include <dispatch/dispatch.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

void chime_execution_initialize(void);

chime_object_t* execution_context_sleep(chime_object_t* instance, chime_object_t* duration);

chime_object_t* execution_context_sync(chime_object_t* instance, chime_object_t* function);
chime_object_t* execution_context_async(chime_object_t* instance, chime_object_t* function);
chime_object_t* execution_context_wait(chime_object_t* instance);

#ifdef PLATFORM_MAC_OS_X
dispatch_queue_t execution_context_get_dispatch_queue(chime_object_t* instance);
void             execution_context_set_dispatch_queue(chime_object_t* instance, dispatch_queue_t queue);
dispatch_group_t execution_context_get_dispatch_group(chime_object_t* instance);
void             execution_context_set_dispatch_group(chime_object_t* instance, dispatch_group_t group);
#endif

#ifdef __cplusplus
}
#endif

#endif
