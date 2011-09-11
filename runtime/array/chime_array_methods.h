// Chime Runtime: chime_array_methods.h

#ifndef CHIME_ARRAY_METHODS_H
#define CHIME_ARRAY_METHODS_H

#include "runtime/object/chime_object.h"

#ifdef __cplusplus
extern "C" {
#endif

chime_object_t* array_class_new(chime_class_t* klass);
chime_object_t* array_to_string(chime_object_t* instance);
chime_object_t* array_length(chime_object_t* instance);
chime_object_t* array_add(chime_object_t* instance, chime_object_t* object);
chime_object_t* array_indexer_get(chime_object_t* instance, chime_object_t* index);

#ifdef __cplusplus
}
#endif

#endif // CHIME_ARRAY_METHODS_H
