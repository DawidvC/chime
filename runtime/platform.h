// Chime Runtime: platform.h

#ifndef PLATFORM_H
#define PLATFORM_H

// platform identification
#if defined(__APPLE__)
#   define PLATFORM_MAC_OS_X 1
#   define PLATFORM_UNIX     1
#elif defined(__linux__)
#   define PLATFORM_LINUX    1
#   define PLATFORM_UNIX     1
#endif

#ifdef __cplusplus
extern "C" {
#endif

// memory allocation
#include "runtime/core/object/chime_object_internal.h"

#define CHIME_ALLOCATION_UNIT sizeof(struct _chime_object)

#include <stdlib.h>
#define chime_allocate(x)       malloc(x*CHIME_ALLOCATION_UNIT)
#define chime_deallocate(x)     free(x)
#define chime_malloc(x)         malloc(x)
#define chime_free(x)           free(x)

#include <string.h>
#define chime_zero_memory(x, n)   bzero(x, n*CHIME_ALLOCATION_UNIT)
#define chime_memcpy(dst, src, n) memcpy(dst, src, n)

// atomic operations
#if defined(PLATFORM_MAC_OS_X)
#   include <libkern/OSAtomic.h>

#   define chime_atomic_increment32_barrier(x) OSAtomicIncrement32(x)
#   define chime_atomic_decrement32_barrier(x) OSAtomicDecrement32Barrier(x)
#endif

#ifdef __cplusplus
}
#endif

#endif // PLATFORM_H
