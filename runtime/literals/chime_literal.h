// Chime Runtime: chime_literals.h

#ifndef CHIME_LITERALS
#define CHIME_LITERALS

#include "runtime/object/chime_object.h"

#ifdef __cplusplus
extern "C" {
#endif

// pointer tagging!
// Systems will align pointers to 32 bit values by 4 byte boundaries, and this means
// the bottom two bits will never actually be used.
// 00 = regular object
// 01 = integer literal
// 10 = "special literal"
#define CHIME_LITERAL_NULL  ((chime_object_t*)0x00000000)
#define CHIME_LITERAL_TRUE  ((chime_object_t*)0x00000006)
#define CHIME_LITERAL_FALSE ((chime_object_t*)0x0000000A)

void            chime_literal_initialize(void);

chime_object_t* chime_literal_encode_integer(signed long value);
signed long     chime_literal_decode_integer(chime_object_t* object);

chime_object_t* chime_literal_encode_boolean(unsigned char value);
unsigned char   chime_literal_decode_boolean(chime_object_t* object);

unsigned char chime_object_is_literal(chime_object_t* object);
unsigned char chime_object_is_integer(chime_object_t* object);
unsigned char chime_object_is_boolean(chime_object_t* instance);

#ifdef __cplusplus
}
#endif

#endif
