// Chime Runtime: chime_object.h

#ifndef CHIME_OBJECT_H
#define CHIME_OBJECT_H

#include "runtime/class/chime_class.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _chime_object chime_object_t;

typedef chime_object_t* (*chime_function_arg0_t)(chime_object_t*);
typedef chime_object_t* (*chime_function_arg1_t)(chime_object_t*, chime_object_t*);
typedef chime_object_t* (*chime_function_arg2_t)(chime_object_t*, chime_object_t*, chime_object_t*);
typedef chime_object_t* (*chime_function_arg3_t)(chime_object_t*, chime_object_t*, chime_object_t*, chime_object_t*);

typedef enum {
    CHIME_OBJECT_INVALID_TYPE = 0,
    CHIME_OBJECT_TYPE         = 1,
    CHIME_NULL_TYPE           = 2,
    CHIME_INTEGER_TYPE        = 3,
    CHIME_BOOLEAN_TYPE        = 4
} chime_object_type_t;

void            chime_object_initialize(void);

chime_object_t* chime_object_create_with_name(const char* class_name);
chime_object_t* chime_object_create(chime_class_t* klass);
void            chime_object_destroy(chime_object_t* instance);

void chime_object_retain(chime_object_t* instance);
void chime_object_release(chime_object_t* instance);
int  chime_object_get_retain_count(chime_object_t* instance);

chime_object_type_t chime_object_get_type(chime_object_t* instance);

chime_class_t*  chime_object_get_class(chime_object_t* instance);
chime_object_t* chime_object_get_superclass(chime_object_t* instance);
chime_object_t* chime_object_get_property(chime_object_t* instance, const char* name);
chime_object_t* chime_object_get_attribute(chime_object_t* instance, const char* name);
void            chime_object_set_property(chime_object_t* instance, const char* name, chime_object_t* value);
void            chime_object_set_attribute(chime_object_t* instance, const char* name, chime_object_t* value);

void*           chime_object_resolve_invoke(chime_object_t* instance, const char* name);
chime_object_t* chime_object_invoke_0(chime_object_t* instance, const char* name);
chime_object_t* chime_object_invoke_1(chime_object_t* instance, const char* name, chime_object_t* arg1);
chime_object_t* chime_object_invoke_2(chime_object_t* instance, const char* name, chime_object_t* arg1, chime_object_t* arg2);
chime_object_t* chime_object_invoke_3(chime_object_t* instance, const char* name, chime_object_t* arg1, chime_object_t* arg2, chime_object_t* arg3);

#ifdef __cplusplus
}
#endif

#endif // CHIME_OBJECT_H
