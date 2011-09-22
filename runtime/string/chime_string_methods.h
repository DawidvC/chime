// Chime Runtime: chime_string_methods.h

#ifndef CHIME_STRING_METHODS_H
#define CHIME_STRING_METHODS_H

#include "runtime/object/chime_object.h"

#ifdef __cplusplus
extern "C" {
#endif

chime_object_t* string_class_new(chime_class_t* klass);

chime_object_t* string_to_string(chime_object_t* instance);
chime_object_t* string_print(chime_object_t* instance);
chime_object_t* string_compare(chime_object_t* instance, chime_object_t* other);
chime_object_t* string_indexer_get(chime_object_t* instance, chime_object_t* index);
chime_object_t* string_length_get(chime_object_t* instance);

#ifdef __cplusplus
}
#endif

#endif // CHIME_OBJECT_METHODS_H
