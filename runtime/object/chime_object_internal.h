// Chime Runtime: chime_object_internal.h

#ifndef CHIME_OBJECT_INTERNAL_H
#define CHIME_OBJECT_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "runtime/collections/chime_dictionary.h"

struct _chime_object {
    // bookkeeping
    unsigned long flags;
    unsigned long reference_count;
    
    // inheritance
    struct _chime_object* self_class;
    struct _chime_object* super_class;
    
    // properties
    chime_dictionary_t* properties;
};

#ifdef __cplusplus
}
#endif

#endif // CHIME_OBJECT_INTERNAL_H
