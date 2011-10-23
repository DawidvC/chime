// Chime Runtime: chime_runtime.h

#ifndef CHIME_RUNTIME
#define CHIME_RUNTIME

#include "runtime/class/chime_class.h"
#include "runtime/object/chime_object.h"
#include "runtime/literals/chime_literal.h"
#include "runtime/closure/chime_closure.h"

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned char chime_log_level;

void chime_runtime_initialize(void);
void chime_runtime_destroy(void);

chime_class_t*  chime_runtime_get_class(const char* name);
char*           chime_runtime_get_class_name(chime_class_t* klass);

chime_object_t* chime_runtime_create_class(const char* name, chime_object_t* superclass);
chime_object_t* chime_runtime_create_trait(const char* name);
void            chime_runtime_set_instance_method(chime_object_t* klass, const char* name, void* function);
void            chime_runtime_set_class_method(chime_object_t* klass, const char* name, void* function);
chime_object_t* chime_runtime_instantiate(chime_class_t* klass);

chime_object_t* chime_runtime_load(const char* name);

#ifdef __cplusplus
}
#endif

#endif
