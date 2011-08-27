// Chime Runtime: chime_closure_internal.h

#ifndef CHIME_CLOSURE_INTERNAL_H
#define CHIME_CLOSURE_INTERNAL_H

#include "runtime/object/chime_object_internal.h"

struct _chime_closure {
    struct _chime_object object; // make it an object
    void*  function;             // with an extra field for its function
};

#endif // CHIME_CLOSURE_INTERNAL_H
