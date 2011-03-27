// Chime Runtime: chime_object_internal.h

#ifndef CHIME_OBJECT_INTERNAL
#define CHIME_OBJECT_INTERNAL

#include "collections/chime_dictionary.h"

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

#endif
