// Chime Runtime: chime_boolean.h

#ifndef CHIME_BOOLEAN_H
#define CHIME_BOOLEAN_H

#include "runtime/types.h"

#ifdef __cplusplus
extern "C" {
#endif

void chime_boolean_initialize(void);

chime_object_t* chime_boolean_encode(bool value);
bool            chime_boolean_decode(chime_object_t* object);

chime_object_t* boolean_to_string(chime_object_t* instance);
chime_object_t* boolean_compare(chime_object_t* instance, chime_object_t* other);

#ifdef __cplusplus
}
#endif

#endif // CHIME_BOOLEAN_H
