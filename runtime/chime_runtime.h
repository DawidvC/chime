// Chime Runtime: chime_runtime.h

#ifndef CHIME_RUNTIME
#define CHIME_RUNTIME

#include "chime_object.h"
#include "chime_literals.h"

#ifdef __cplusplus
extern "C" {
#endif

void chime_runtime_initialize(void);
void chime_runtime_destroy(void);

chime_object_t* chime_runtime_create_class(const char* name, chime_object_t* super_class);
chime_object_t* chime_runtime_get_class(const char* name);

#ifdef __cplusplus
}
#endif

#endif
