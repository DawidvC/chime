// Chime Runtime: chime_range_methods.h

#ifndef CHIME_RANGE_METHODS_H
#define CHIME_RANGE_METHODS_H

#include "runtime/object/chime_object.h"

#ifdef __cplusplus
extern "C" {
#endif

chime_object_t* range_start_get(chime_object_t* instance);
chime_object_t* range_start_set(chime_object_t* instance, chime_object_t* start);
chime_object_t* range_end_get(chime_object_t* instance);
chime_object_t* range_end_set(chime_object_t* instance, chime_object_t* end);

#ifdef __cplusplus
}
#endif

#endif // CHIME_RANGE_METHODS_H
