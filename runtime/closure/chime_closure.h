// Chime Runtime: chime_closure.h

#ifndef CHIME_CLOSURE_H
#define CHIME_CLOSURE_H

#include "runtime/object/chime_object.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _chime_closure chime_closure_t;

void chime_closure_initialize(void);

chime_closure_t* chime_closure_create(void* function);
void             chime_closure_destroy(chime_closure_t* closure);

chime_object_t*  chime_closure_invoke(chime_closure_t* closure, chime_object_t* argument_array);

#ifdef __cplusplus
}
#endif

#endif // CHIME_CLOSURE_H
