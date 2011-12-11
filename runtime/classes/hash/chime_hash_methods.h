// Chime Runtime: chime_hash_methods.h

#ifndef CHIME_HASH_METHODS_H
#define CHIME_HASH_METHODS_H

#include "runtime/types.h"

#ifdef __cplusplus
extern "C" {
#endif

chime_object_t* hash_length(chime_object_t* instance);
chime_object_t* hash_indexer_get(chime_object_t* instance, chime_object_t* index);
chime_object_t* hash_indexer_set(chime_object_t* instance, chime_object_t* index, chime_object_t* value);

#ifdef __cplusplus
}
#endif

#endif // CHIME_HASH_METHODS_H
