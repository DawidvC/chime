// Chime Runtime: chime_integer.h

#ifndef CHIME_INTEGER_H
#define CHIME_INTEGER_H

#include "runtime/types.h"

#ifdef __cplusplus
extern "C" {
#endif

void chime_integer_initialize(void);

chime_object_t* chime_integer_encode(signed long value);
signed long     chime_integer_decode(chime_object_t* object);

chime_object_t* integer_to_string(chime_object_t* instance);
chime_object_t* integer_times(chime_object_t* instance, chime_object_t* function);
chime_object_t* integer_add(chime_object_t* instance, chime_object_t* other);
chime_object_t* integer_subtract(chime_object_t* instance, chime_object_t* other);
chime_object_t* integer_multiply(chime_object_t* instance, chime_object_t* other);
chime_object_t* integer_divide(chime_object_t* instance, chime_object_t* other);
chime_object_t* integer_modulus(chime_object_t* instance, chime_object_t* other);
chime_object_t* integer_compare(chime_object_t* instance, chime_object_t* other);

#ifdef __cplusplus
}
#endif

#endif // CHIME_INTEGER_H
