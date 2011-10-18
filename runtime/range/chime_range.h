// Chime Runtime: chime_range.h

#ifndef CHIME_RANGE_H
#define CHIME_RANGE_H

#include "runtime/object/chime_object.h"

#ifdef __cplusplus
extern "C" {
#endif

void chime_range_initialize(void);

chime_object_t* chime_range_create(chime_object_t* start, chime_object_t* end);

#ifdef __cplusplus
}
#endif

#endif // CHIME_RANGE_H
