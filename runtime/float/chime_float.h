// Chime Runtime: chime_float.h

#ifndef CHIME_FLOAT_H
#define CHIME_FLOAT_H

#include "runtime/object/chime_object.h"

#ifdef __cplusplus
extern "C" {
#endif

void chime_float_initialize(void);

chime_object_t* chime_float_create(double value);
void            chime_float_destroy(chime_object_t* instance);

void chime_float_set_value(chime_object_t* instance, double value);
double chime_float_get_value(chime_object_t* instance);

#ifdef __cplusplus
}
#endif

#endif // CHIME_RANGE_H
