// Chime Runtime: support.h

#ifndef SUPPORT_H
#define SUPPORT_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
#ifndef bool
typedef unsigned char bool;

#define true (1)
#define false (0)
#endif // bool
#endif // __cplusplus

// memory allocation
#include "runtime/object/chime_object_internal.h"

#define CHIME_ALLOCATION_UNIT sizeof(struct _chime_object)

#include <stdlib.h>
#define chime_allocate(x)   malloc(x*CHIME_ALLOCATION_UNIT)
#define chime_deallocate(x) free(x)

// atomic operations
#include <libkern/OSAtomic.h>
#define chime_atomic_increment32_barrier(x) OSAtomicIncrement32(x)
#define chime_atomic_decrement32_barrier(x) OSAtomicDecrement32Barrier(x)

#ifdef __cplusplus
}
#endif

#endif // SUPPORT_H
