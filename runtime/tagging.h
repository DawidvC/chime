// Chime Runtime: tagging.h

#ifndef CHIME_TAGGING_H
#define CHIME_TAGGING_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

// pointer tagging!
// Systems will align pointers to 32 bit values by 4 byte boundaries, and this means
// the bottom two bits will never actually be used.  We can make use of these to
// identify special objects
// 00 = regular object
// 01 = integer literal
// 10 = encoded tag (need to look at more bits)
// 11 = raw data block pointer

typedef enum {
    chime_tagged_object,
    chime_tagged_integer,
    chime_tagged_encoded,
    chime_tagged_data
} chime_tagged_t;

bool            chime_object_is_tagged(chime_object_t* object);

chime_object_t* chime_tag_encode_integer(signed long value);
signed long     chime_tag_decode_integer(chime_object_t* object);

chime_object_t* chime_tag_encode_raw_block(void* data);
void*           chime_tag_decode_raw_block(chime_object_t* tagged_object);

// other types that require more internal poking around
chime_object_t* chime_tag_encode_boolean(bool value);
bool            chime_tag_decode_boolean(chime_object_t* object);

unsigned char chime_object_is_literal(chime_object_t* object);
unsigned char chime_object_is_integer(chime_object_t* object);
unsigned char chime_object_is_boolean(chime_object_t* instance);

#ifdef __cplusplus
}
#endif

#endif // CHIME_TAGGING_H
