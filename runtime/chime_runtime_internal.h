// Chime Runtime: chime_runtime_internal.h

#ifndef CHIME_RUNTIME_INTERNAL
#define CHIME_RUNTIME_INTERNAL

#include "chime_object_internal.h"

extern chime_object_t* _object_class;
extern chime_object_t* _string_class;

void chime_initialize_class_heirarchy(void);

#endif
