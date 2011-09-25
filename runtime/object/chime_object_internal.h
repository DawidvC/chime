// Chime Runtime: chime_object_internal.h

#ifndef CHIME_OBJECT_INTERNAL_H
#define CHIME_OBJECT_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "runtime/class/chime_class.h"
#include "runtime/collections/chime_dictionary.h"

struct _chime_object {
    struct _chime_class* self_class;
    unsigned long        flags;
    chime_dictionary_t*  methods;
    chime_dictionary_t*  variables;
};

chime_dictionary_t* chime_object_get_methods(struct _chime_object* instance);
chime_dictionary_t* chime_object_get_attributes(struct _chime_object* instance);
void                chime_object_print(struct _chime_object* instance);

#ifdef __cplusplus
}
#endif

#endif // CHIME_OBJECT_INTERNAL_H
