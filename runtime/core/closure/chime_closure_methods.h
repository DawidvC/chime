// Chime Runtime: chime_closure_methods.h

#ifndef CHIME_CLOSURE_METHODS_H
#define CHIME_CLOSURE_METHODS_H

#include "runtime/types.h"

#ifdef __cplusplus
extern "C" {
#endif

chime_object_t* closure_to_string(chime_object_t* instance);
chime_object_t* closure_call_0(chime_object_t* instance);
chime_object_t* closure_call_1(chime_object_t* instance, chime_object_t* arg1);
chime_object_t* closure_call_2(chime_object_t* instance, chime_object_t* arg1, chime_object_t* arg2);

#ifdef __cplusplus
}
#endif

#endif // CHIME_CLOSURE_METHODS_H
