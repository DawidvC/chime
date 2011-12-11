// Chime Runtime: chime_null.h

#ifndef CHIME_NULL_H
#define CHIME_NULL_H

#include "runtime/types.h"

#ifdef __cplusplus
extern "C" {
#endif

void chime_null_initialize(void);

chime_object_t* null_to_string(chime_object_t* instance);

#ifdef __cplusplus
}
#endif

#endif // CHIME_NULL_H
