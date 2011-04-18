// Chime Runtime: chime_literals.h

#ifndef CHIME_LITERALS
#define CHIME_LITERALS

#include "chime_object.h"

#ifdef __cplusplus
extern "C" {
#endif

// pointer tagging!
// Systems will align pointers to 32 bit values by 4 byte boundaries, and this means
// the bottom two bits will never actually be used.
#define CHIME_LITERAL_NULL    ((chime_object_t*)0x00000000)
#define CHIME_LITERAL_TRUE   0x00000007
#define CHIME_LITERAL_FALSE  0x0000000B

void            chime_literal_initialize(void);

chime_object_t* chime_literal_encode_integer(signed long value);
signed long     chime_literal_decode_integer(chime_object_t* object);

chime_object_t* chime_literal_encode_string(const char* value);
char*           chime_literal_decode_string(chime_object_t* instance);

unsigned char chime_object_is_literal(chime_object_t* object);
unsigned char chime_object_is_integer(chime_object_t* object);
unsigned char chime_object_is_string(chime_object_t* instance);

#ifdef __cplusplus
}
#endif

#endif
