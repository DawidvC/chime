// Chime Runtime: chime_reference.h

#ifndef CHIME_REFERENCE_H
#define CHIME_REFERENCE_H

#include "runtime/object/chime_object.h"

#ifdef __cplusplus
extern "C" {
#endif

void            chime_reference_initialize(void);

chime_object_t* chime_reference_create(chime_object_t* object);
void            chime_reference_destroy(chime_object_t* reference);

chime_object_t* chime_reference_get(chime_object_t* reference);
void            chime_reference_set(chime_object_t* reference, chime_object_t* object);

#ifdef __cplusplus
}
#endif

#endif // CHIME_REFERENCE_H
