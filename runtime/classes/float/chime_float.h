// Chime Runtime: chime_float.h

#ifndef CHIME_FLOAT_H
#define CHIME_FLOAT_H

#include "runtime/types.h"

#ifdef __cplusplus
extern "C" {
#endif

void chime_float_initialize(void);

chime_object_t* chime_float_create(double value);
void            chime_float_destroy(chime_object_t* instance);

void   chime_float_set_value(chime_object_t* instance, double value);
double chime_float_get_value(chime_object_t* instance);

chime_object_t* float_compare(chime_object_t* instance, chime_object_t* other);
chime_object_t* float_add(chime_object_t* instance, chime_object_t* other);
chime_object_t* float_subtract(chime_object_t* instance, chime_object_t* other);
chime_object_t* float_divide(chime_object_t* instance, chime_object_t* other);
chime_object_t* float_multiply(chime_object_t* instance, chime_object_t* other);

#ifdef __cplusplus
}
#endif

#endif // CHIME_RANGE_H
