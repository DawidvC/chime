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

chime_object_t* chime_runtime_create_class(const char* name, chime_object_t* super_class);
chime_object_t* chime_runtime_get_class(const char* name);
char*           chime_runtime_get_class_name(chime_object_t* instance);

void            chime_runtime_set_instance_method(chime_object_t* class_instance, const char* name, chime_function_t function, unsigned long arity);

#ifdef __cplusplus
}
#endif

#endif
