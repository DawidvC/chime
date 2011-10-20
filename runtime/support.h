// Chime Runtime: support.h

#ifndef SUPPORT_H
#define SUPPORT_H

#ifdef __cplusplus
extern "C" {
#endif

// memory allocation
#include "runtime/object/chime_object_internal.h"

#define CHIME_ALLOCATION_UNIT sizeof(struct _chime_object)

#include <stdlib.h>
#define chime_allocate(x)   malloc(x*CHIME_ALLOCATION_UNIT)
#define chime_deallocate(x) free(x)

#ifndef __cplusplus
typedef unsigned char bool;

#define true (1)
#define false (0)
#endif

#ifdef __cplusplus
}
#endif

#endif // SUPPORT_H
