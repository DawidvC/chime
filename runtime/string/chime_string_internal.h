// Chime Runtime: chime_string_internal.h

#ifndef CHIME_STRING_INTERNAL_H
#define CHIME_STRING_INTERNAL_H

#include "runtime/collections/chime_runtime_array.h"
#include "runtime/object/chime_object_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _chime_string {
    struct _chime_object   object;         // make it an object
    chime_runtime_array_t* internal_array;
    char*                  buffer;
    signed long            length;
};
    
chime_runtime_array_t* string_get_internal_array(chime_object_t* instance);

char*                  string_get_buffer(chime_object_t* instance);
void                   string_set_buffer(chime_object_t* instance, char* value);

chime_object_t*        string_length_set(chime_object_t* instance, signed long length);

#ifdef __cplusplus
}
#endif

#endif // CHIME_STRING_INTERNAL_H
