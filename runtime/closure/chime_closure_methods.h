// Chime Runtime: chime_closure_methods.h

#ifndef CHIME_CLOSURE_METHODS_H
#define CHIME_CLOSURE_METHODS_H

#include "runtime/object/chime_object.h"

#ifdef __cplusplus
extern "C" {
#endif

chime_object_t* closure_to_string(chime_object_t* instance);
chime_object_t* closure_call(chime_object_t* instance, chime_object_t* argument_array);

#ifdef __cplusplus
}
#endif

#endif // CHIME_CLOSURE_METHODS_H
