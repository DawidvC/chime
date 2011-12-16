// Chime Runtime: chime_object_methods.h

#ifndef CHIME_OBJECT_METHODS_H
#define CHIME_OBJECT_METHODS_H

#include "runtime/types.h"

#ifdef __cplusplus
extern "C" {
#endif

chime_object_t* object_initialize(chime_object_t* instance);
chime_object_t* object_finalize(chime_object_t* instance);
chime_object_t* object_class(chime_object_t* instance);
chime_object_t* object_superclass(chime_object_t* instance);
chime_object_t* object_methods(chime_object_t* instance);
chime_object_t* object_invoke(chime_object_t* instance, chime_object_t* method_name);
chime_object_t* object_hash(chime_object_t* instance);
chime_object_t* object_to_string(chime_object_t* instance);
chime_object_t* object_nil(chime_object_t* instance);
chime_object_t* object_methods(chime_object_t* instance);
chime_object_t* object_retain_count(chime_object_t* instance);
chime_object_t* object_equals(chime_object_t* instance, chime_object_t* other);
chime_object_t* object_case_compare(chime_object_t* instance, chime_object_t* other);

chime_object_t* method_name(chime_object_t* instance);

#ifdef __cplusplus
}
#endif

#endif // CHIME_OBJECT_METHODS_H
