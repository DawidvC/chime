// Chime Runtime: chime_literals.c

#include "chime_literals.h"
#include <stdio.h>

chime_object_t* chime_literal_encode_integer(signed long value)
{
    unsigned long encoded_value;
    
    encoded_value = value << 2;
    encoded_value = encoded_value | 0x01;
    
    if (value < 0)
        encoded_value = encoded_value | 0x02;
    
    return (chime_object_t*)encoded_value;
}

signed long chime_literal_decode_integer(chime_object_t* object)
{
    signed long value;
    
    value = (unsigned long)object >> 2;
    
    // this little trick depends on the size of the value (32 or 64 bits)
    if ((unsigned long)object & 0x02)
        value = value | 0xC000000000000000;
    
    return value;
}

unsigned char chime_object_is_literal(chime_object_t* object)
{
    return (unsigned long)object & 0x3;
}

unsigned char chime_object_is_integer(chime_object_t* object)
{
    return (unsigned long)object & 0x1;
}
