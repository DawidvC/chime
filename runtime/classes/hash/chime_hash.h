// Chime Runtime: chime_hash.h

#ifndef CHIME_HASH_H
#define CHIME_HASH_H

#include "runtime/types.h"

#ifdef __cplusplus
extern "C" {
#endif

void chime_hash_initialize(void);

chime_object_t* chime_hash_create(void);

chime_object_t* chime_hash_set(chime_object_t* instance, chime_object_t* key, chime_object_t* value);

#ifdef __cplusplus
}
#endif

#endif // CHIME_HASH_H
