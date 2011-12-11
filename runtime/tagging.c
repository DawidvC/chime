#include "tagging.h"

bool chime_object_is_tagged(chime_object_t* object)
{
    return ((unsigned long)object & 0x3) || (object == CHIME_LITERAL_NULL);
}

chime_object_t* chime_tag_encode_integer(signed long value)
{
    unsigned long encoded_value;
    
    encoded_value = value << 2;
    encoded_value = encoded_value | 0x01;
    
    if (value < 0)
        encoded_value = encoded_value | 0x02;
    
    return (chime_object_t*)encoded_value;
}

signed long chime_tag_decode_integer(chime_object_t* object)
{
    signed long value;
    
    value = (unsigned long)object >> 2;
    
    // this little trick depends on the size of the value (32 or 64 bits)
    if ((unsigned long)object & 0x02)
        value = value | 0xC000000000000000;
    
    return value;
}

chime_object_t* chime_tag_encode_raw_block(void* data)
{
    return (chime_object_t*)((unsigned long)data | 0x03);
}

void* chime_tag_decode_raw_block(chime_object_t* tagged_object)
{
    return (void*)((unsigned long)tagged_object & ~0x03);
}

// other types that require more internal poking around
chime_object_t* chime_tag_encode_boolean(bool value)
{
    return value ? CHIME_LITERAL_TRUE : CHIME_LITERAL_FALSE;
}

bool chime_tag_decode_boolean(chime_object_t* object)
{
    if (object == CHIME_LITERAL_TRUE)
        return true;
    
    return false;
}



unsigned char chime_object_is_literal(chime_object_t* object)
{
    return ((unsigned long)object & 0x3) || (object == CHIME_LITERAL_NULL);
}

unsigned char chime_object_is_integer(chime_object_t* object)
{
    return (unsigned long)object & 0x1;
}

unsigned char chime_object_is_boolean(chime_object_t* instance)
{
    return instance == CHIME_LITERAL_TRUE || instance == CHIME_LITERAL_FALSE;
}
