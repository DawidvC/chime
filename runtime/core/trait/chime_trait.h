// Chime Runtime: chime_trait.h

#ifndef CHIME_TRAIT_H
#define CHIME_TRAIT_H

#include "runtime/types.h"

#ifdef __cplusplus
extern "C" {
#endif

void chime_trait_initialize(void);

chime_class_t* chime_trait_create(const char* name);

#ifdef __cplusplus
}
#endif

#endif // CHIME_TRAIT_H
