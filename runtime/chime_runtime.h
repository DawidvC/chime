// Chime Runtime: chime_runtime.h

#ifndef CHIME_RUNTIME
#define CHIME_RUNTIME

#include "chime_object.h"
#include "chime_string.h"
#include "chime_literals.h"

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned char chime_log_level;

void chime_runtime_initialize(void);
void chime_runtime_destroy(void);

chime_object_t* chime_runtime_create_class(const char* name, chime_object_t* superclass);
chime_object_t* chime_runtime_get_class(const char* name);
char*           chime_runtime_get_class_name(chime_object_t* instance);

#ifdef __cplusplus
}
#endif

#endif
