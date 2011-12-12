// Chime Library: process.h

#ifndef CHIME_PROCESS_H
#define CHIME_PROCESS_H

#include "runtime/chime_runtime.h"

#ifdef __cplusplus
extern "C" {
#endif

void chime_process_initialize(void);

chime_object_t* process_run(chime_object_t* instance);

#ifdef __cplusplus
}
#endif

#endif // CHIME_PROCESS_H
