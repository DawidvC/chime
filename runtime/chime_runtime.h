// Chime Runtime: chime_runtime.h

#ifndef CHIME_RUNTIME
#define CHIME_RUNTIME

#include "runtime/object/chime_object.h"
#include "runtime/literals/chime_literal.h"

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned char chime_log_level;

void chime_runtime_initialize(void);
void chime_runtime_destroy(void);

chime_object_t* chime_runtime_create_class(const char* name, chime_object_t* superclass);
chime_object_t* chime_runtime_create_object_subclass(const char* name);
chime_object_t* chime_runtime_get_class(const char* name);
char*           chime_runtime_get_class_name(chime_object_t* class_instance);

chime_object_t* chime_runtime_load(const char* name);

#ifdef __cplusplus
}
#endif

#endif
