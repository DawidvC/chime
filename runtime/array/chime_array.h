// Chime Runtime: chime_array.h

#ifndef CHIME_ARRAY_H
#define CHIME_ARRAY_H

#include "runtime/object/chime_object.h"

#ifdef __cplusplus
extern "C" {
#endif

void chime_array_initialize(void);

chime_object_t* chime_array_create(void);
void            chime_array_destroy(chime_object_t* instance);
chime_object_t* chime_array_create_with_length(unsigned long initial_length);

chime_object_t* chime_array_append(chime_object_t* instance, chime_object_t* object);

#ifdef __cplusplus
}
#endif

#endif // CHIME_ARRAY_H
