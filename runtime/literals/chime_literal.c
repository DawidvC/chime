// Chime Runtime: chime_literal.c

#include "chime_literal.h"
#include "runtime/chime_runtime.h"
#include "chime_literal_methods.h"
#include <stdio.h>
#include <assert.h>

void chime_literal_initialize(void)
{
    chime_class_t* klass;
        
    klass = chime_class_create_object_subclass("Null");
    chime_class_set_instance_method(klass, "print", null_print);
    
    klass = chime_class_create_object_subclass("Integer");
    chime_class_set_instance_method(klass, "print", integer_print);
    chime_class_set_instance_method(klass, "+",     integer_add);
    chime_class_set_instance_method(klass, "*",     integer_multiply);
    chime_class_set_instance_method(klass, "<=>",   integer_compare);
    
    klass = chime_class_create_object_subclass("Boolean");
    chime_class_set_instance_method(klass, "print", boolean_print);
    chime_class_set_instance_method(klass, "<=>",   boolean_compare);
}

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

chime_object_t* chime_literal_encode_boolean(unsigned char value)
{
    return value ? CHIME_LITERAL_TRUE : CHIME_LITERAL_FALSE;
}

unsigned char chime_literal_decode_boolean(chime_object_t* object)
{
    if (object == CHIME_LITERAL_TRUE)
        return 1;
    
    return 0;
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
