// Chime Runtime: chime_comparable_methods.h

#ifndef CHIME_COMPARABLE_METHODS_H
#define CHIME_COMPARABLE_METHODS_H

#include "runtime/object/chime_object.h"

#ifdef __cplusplus
extern "C" {
#endif

chime_object_t* comparable_equals(chime_object_t* instance, chime_object_t* other);
chime_object_t* comparable_less_than(chime_object_t* instance, chime_object_t* other);
chime_object_t* comparable_less_or_equals(chime_object_t* instance, chime_object_t* other);
chime_object_t* comparable_greater_than(chime_object_t* instance, chime_object_t* other);
chime_object_t* comparable_greater_or_equals(chime_object_t* instance, chime_object_t* other);

#ifdef __cplusplus
}
#endif

#endif // CHIME_COMPARABLE_METHODS_H
