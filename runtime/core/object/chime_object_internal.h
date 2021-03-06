// Chime Runtime: chime_object_internal.h

#ifndef CHIME_OBJECT_INTERNAL_H
#define CHIME_OBJECT_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "runtime/types.h"
#include "runtime/support.h"

enum _object_flags {
    ObjectIsClass      = 0x00000001,
    ObjectHasVariables = 0x00000002
};

struct _chime_object {
    struct _chime_class*  self_class;
    volatile unsigned int flags;
    volatile int          retain_count;
    chime_dictionary_t*   methods;
    chime_dictionary_t*   variables;
};

struct _chime_object* chime_object_raw_create(chime_class_t* object_class, unsigned int struct_size, unsigned char create_variables);

chime_dictionary_t*   chime_object_get_methods(struct _chime_object* instance);
chime_dictionary_t*   chime_object_get_attributes(struct _chime_object* instance);
void                  chime_object_print(struct _chime_object* instance);

#ifdef __cplusplus
}
#endif

#endif // CHIME_OBJECT_INTERNAL_H
