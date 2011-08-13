// Chime Runtime: chime_object_methods.h

#ifndef CHIME_OBJECT_METHODS_H
#define CHIME_OBJECT_METHODS_H

#include "runtime/object/chime_object.h"

#ifdef __cplusplus
extern "C" {
#endif

chime_object_t* object_class(chime_object_t* instance);
chime_object_t* object_to_string(chime_object_t* instance);
chime_object_t* object_methods(chime_object_t* instance);
chime_object_t* object_equals(chime_object_t* instance, chime_object_t* other);

#ifdef __cplusplus
}
#endif

#endif // CHIME_OBJECT_METHODS_H
