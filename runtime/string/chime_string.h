// Chime Runtime: chime_string.h

#ifndef CHIME_STRING
#define CHIME_STRING

#include "runtime/object/chime_object.h"
#include "runtime/support.h"

#ifdef __cplusplus
extern "C" {
#endif

#define STRING_STORAGE_UNIT           8
#define BYTES_PER_STRING_STORAGE_UNIT (STRING_STORAGE_UNIT * CHIME_ALLOCATION_UNIT)
#define STRING_EMPTY_BUFFER_FLAG      ((char*)1)

typedef struct _chime_string chime_string_t;

void            chime_string_initialize(void);

chime_object_t* chime_string_create(void);
void            chime_string_destroy(chime_object_t* instance);

chime_object_t* chime_string_create_with_c_string(const char* string, unsigned int length);
char*           chime_string_to_c_string(chime_object_t* instance);

#ifdef __cplusplus
}
#endif

#endif
