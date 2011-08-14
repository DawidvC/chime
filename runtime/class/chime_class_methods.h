// Chime Runtime: chime_class_methods.h

#ifndef CHIME_CLASS_METHODS_H
#define CHIME_CLASS_METHODS_H

#include "runtime/object/chime_object.h"

#ifdef __cplusplus
extern "C" {
#endif

chime_object_t* class_name(chime_object_t* instance);
chime_object_t* class_compare(chime_object_t* instance, chime_object_t* other);

#ifdef __cplusplus
}
#endif

#endif // CHIME_CLASS_METHODS_H
