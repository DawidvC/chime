// Chime Runtime: chime_array_methods.h

#ifndef CHIME_ARRAY_METHODS_H
#define CHIME_ARRAY_METHODS_H

#include "runtime/types.h"

#ifdef __cplusplus
extern "C" {
#endif

chime_object_t* array_class_new(chime_class_t* klass);
chime_object_t* array_to_string(chime_object_t* instance);
chime_object_t* array_length(chime_object_t* instance);
chime_object_t* array_contains(chime_object_t* instance, chime_object_t* value);
chime_object_t* array_append(chime_object_t* instance, chime_object_t* object);
chime_object_t* array_each(chime_object_t* instance, chime_object_t* function);
chime_object_t* array_map(chime_object_t* instance, chime_object_t* function);
chime_object_t* array_indexer_get(chime_object_t* instance, chime_object_t* index);

#ifdef __cplusplus
}
#endif

#endif // CHIME_ARRAY_METHODS_H
