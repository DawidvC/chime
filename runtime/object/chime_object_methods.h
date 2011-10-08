// Chime Runtime: chime_object_methods.h

#ifndef CHIME_OBJECT_METHODS_H
#define CHIME_OBJECT_METHODS_H

#include "runtime/object/chime_object.h"

#ifdef __cplusplus
extern "C" {
#endif

chime_object_t* object_class(chime_object_t* instance);
chime_object_t* object_methods(chime_object_t* instance);
chime_object_t* object_invoke(chime_object_t* instance, chime_object_t* method_name);
chime_object_t* object_to_string(chime_object_t* instance);
chime_object_t* object_methods(chime_object_t* instance);
chime_object_t* object_equals(chime_object_t* instance, chime_object_t* other);
chime_object_t* object_case_compare(chime_object_t* instance, chime_object_t* other);
chime_object_t* object_greater_than(chime_object_t* instance, chime_object_t* other);

chime_object_t* method_name(chime_object_t* instance);

#ifdef __cplusplus
}
#endif

#endif // CHIME_OBJECT_METHODS_H
