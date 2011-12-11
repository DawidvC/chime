// Chime Runtime: platform.h

#ifndef PLATFORM_H
#define PLATFORM_H

// platform identification
#ifdef __APPLE__
#define PLATFORM_MAC_OS_X
#define PLATFORM_UNIX
#endif

#ifdef __cplusplus
extern "C" {
#endif

// memory allocation
#include "runtime/core/object/chime_object_internal.h"

#define CHIME_ALLOCATION_UNIT sizeof(struct _chime_object)

#include <stdlib.h>
#define chime_allocate(x)   malloc(x*CHIME_ALLOCATION_UNIT)
#define chime_deallocate(x) free(x)

// atomic operations
#ifdef PLATFORM_MAC_OS_X
#   include <libkern/OSAtomic.h>

#   define chime_atomic_increment32_barrier(x) OSAtomicIncrement32(x)
#   define chime_atomic_decrement32_barrier(x) OSAtomicDecrement32Barrier(x)
#endif

#ifdef __cplusplus
}
#endif

#endif // PLATFORM_H
