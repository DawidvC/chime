// Chime Runtime: chime_literals.c

#include "chime_literals.h"
#include "chime_runtime.h"
#include "chime_runtime_internal.h"
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

static chime_object_t* IntegerPrint(chime_object_t* instance, const char* method_name, ...)
{
    va_list         arguments;
    chime_object_t* a;
    
    va_start(arguments, method_name);
    
    a = va_arg(arguments, chime_object_t*);
    
    va_end(arguments);
    
    assert(chime_object_is_integer(a));
    
    fprintf(stdout, "<Integer: %ld>\n", chime_literal_decode_integer(a));
    
    return CHIME_LITERAL_NULL;
}

static chime_object_t* IntegerAdd(chime_object_t* instance, const char* method_name, ...)
{
    va_list         arguments;
    chime_object_t* a;
    chime_object_t* b;
    
    va_start(arguments, method_name);
    
    a = va_arg(arguments, chime_object_t*);
    b  = va_arg(arguments, chime_object_t*);
    
    va_end(arguments);
    
    assert(chime_object_is_integer(a));
    
    if (chime_object_is_integer(b))
    {
        signed long result;
        
        result = chime_literal_decode_integer(a) + chime_literal_decode_integer(b);
        
        return chime_literal_encode_integer(result);
    }
    
    return CHIME_LITERAL_NULL;
}

static chime_object_t* IntegerMultiply(chime_object_t* instance, const char* method_name, ...)
{
    va_list         arguments;
    chime_object_t* a;
    chime_object_t* b;
    
    va_start(arguments, method_name);
    
    a = va_arg(arguments, chime_object_t*);
    b  = va_arg(arguments, chime_object_t*);
    
    va_end(arguments);
    
    assert(chime_object_is_integer(a));
    
    if (chime_object_is_integer(b))
    {
        signed long result;
        
        result = chime_literal_decode_integer(a) * chime_literal_decode_integer(b);
        
        return chime_literal_encode_integer(result);
    }
    
    return CHIME_LITERAL_NULL;
}

void chime_literal_initialize(void)
{
    chime_object_t* new_class;
        
    new_class = chime_runtime_create_class("Null", _object_class);
    assert(new_class);
    
    new_class = chime_runtime_create_class("Integer", _object_class);
    assert(new_class);
    
    chime_object_set_function(new_class, "print", IntegerPrint, 1);
    chime_object_set_function(new_class, "+",     IntegerAdd, 2);
    chime_object_set_function(new_class, "*",     IntegerMultiply, 2);
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

unsigned char chime_object_is_literal(chime_object_t* object)
{
    return (unsigned long)object & 0x3;
}

unsigned char chime_object_is_integer(chime_object_t* object)
{
    return (unsigned long)object & 0x1;
}
