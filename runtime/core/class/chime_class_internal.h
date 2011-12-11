// Chime Runtime: chime_class_internal.h

#ifndef CHIME_CLASS_INTERNAL_H
#define CHIME_CLASS_INTERNAL_H

#include "runtime/core/object/chime_object_internal.h"

struct _chime_class {
    struct _chime_object   object;      // make it an object
    struct _chime_class*   super_class;
    chime_runtime_array_t* traits;
};

#endif // CHIME_CLASS_INTERNAL_H
