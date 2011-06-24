// Chime Runtime: chime_literals.c

#include "chime_literals.h"
#include "chime_runtime.h"
#include "chime_runtime_internal.h"
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#pragma mark -
#pragma mark Null Functions
static chime_object_t* NullPrint(chime_object_t* instance, const char* method_name, ...)
{
    assert(instance == CHIME_LITERAL_NULL);
    
    fprintf(stdout, "<Null>\n");
    
    return CHIME_LITERAL_NULL;
}

#pragma mark -
#pragma mark Integer Functions
static chime_object_t* IntegerPrint(chime_object_t* instance, const char* method_name, ...)
{
    assert(chime_object_is_integer(instance));
    
    fprintf(stdout, "<Integer: %ld>\n", chime_literal_decode_integer(instance));
    
    return CHIME_LITERAL_NULL;
}

static chime_object_t* IntegerAdd(chime_object_t* instance, const char* method_name, ...)
{
    va_list         arguments;
    chime_object_t* other;
    
    va_start(arguments, method_name);
    
    other = va_arg(arguments, chime_object_t*);
    
    va_end(arguments);
    
    assert(chime_object_is_integer(instance));
    
    if (chime_object_is_integer(other))
    {
        signed long result;
        
        result = chime_literal_decode_integer(instance) + chime_literal_decode_integer(other);
        
        return chime_literal_encode_integer(result);
    }
    
    return CHIME_LITERAL_NULL;
}

static chime_object_t* IntegerMultiply(chime_object_t* instance, const char* method_name, ...)
{
    va_list         arguments;
    chime_object_t* other;
    
    va_start(arguments, method_name);
    
    other = va_arg(arguments, chime_object_t*);
    
    va_end(arguments);
    
    assert(chime_object_is_integer(instance));
    
    if (chime_object_is_integer(other))
    {
        signed long result;
        
        result = chime_literal_decode_integer(instance) * chime_literal_decode_integer(other);
        
        return chime_literal_encode_integer(result);
    }
    
    return CHIME_LITERAL_NULL;
}

static chime_object_t* IntegerSpaceship(chime_object_t* instance, const char* method_name, ...)
{
    va_list         arguments;
    chime_object_t* other;
    
    va_start(arguments, method_name);
    
    other = va_arg(arguments, chime_object_t*);
    
    va_end(arguments);
    
    assert(chime_object_is_integer(instance) && "First argument to <=> must be an Integer");
    
    if (chime_object_is_integer(other))
    {
        signed long a_value;
        signed long b_value;
        
        a_value = chime_literal_decode_integer(instance);
        b_value = chime_literal_decode_integer(other);
        
        if (a_value == b_value)
            return chime_literal_encode_integer(0);
        
        if (a_value < b_value)
            return chime_literal_encode_integer(-1);
        
        if (a_value > b_value)
            return chime_literal_encode_integer(1);
        
        assert(0 && "Integer <=> wasn't able to compare values");
    }
    
    if (chime_log_level >= 5)
        fprintf(stderr, "[runtime] comparison of integer with non-integer\n");
    
    // if it's not an integer, let's just return nil, meaning comparison is impossible
    return CHIME_LITERAL_NULL;
}

static chime_object_t* IntegerEquality(chime_object_t* instance, const char* method_name, ...)
{
    va_list         arguments;
    chime_object_t* other;
    chime_object_t* result;
    
    va_start(arguments, method_name);
    
    other = va_arg(arguments, chime_object_t*);
    
    va_end(arguments);
    
    assert(chime_object_is_integer(instance) && "First argument to == must be an Integer");
    
    result = chime_object_invoke(instance, "<=>", other);
    
    if (result == CHIME_LITERAL_NULL)
        return CHIME_LITERAL_FALSE;
    
    if (chime_literal_decode_integer(result) == 0)
        return CHIME_LITERAL_TRUE;
    
    return CHIME_LITERAL_FALSE;
}

#pragma mark -
#pragma mark Boolean Function
static chime_object_t* BooleanPrint(chime_object_t* instance, const char* method_name, ...)
{
    assert(chime_object_is_boolean(instance));
    
    fprintf(stdout, "<Boolean: %s>\n", instance == CHIME_LITERAL_TRUE ? "true" : "false");
    
    return CHIME_LITERAL_NULL;
}

static chime_object_t* BooleanSpaceship(chime_object_t* instance, const char* method_name, ...)
{
    va_list         arguments;
    chime_object_t* other;
    
    va_start(arguments, method_name);
    
    other = va_arg(arguments, chime_object_t*);
    
    va_end(arguments);
    
    assert(chime_object_is_boolean(instance) && "First argument to <=> must be a Boolean");
    
    if (chime_object_is_boolean(other))
    {
        if (instance == other)
            return chime_literal_encode_integer(0);
        
        if (other == CHIME_LITERAL_TRUE)
            return chime_literal_encode_integer(-1);
        else
            return chime_literal_encode_integer(1);
    }
    
    if (chime_log_level >= 5)
        fprintf(stderr, "[runtime] comparison of Boolean with non-Boolean\n");
    
    return CHIME_LITERAL_NULL;
}

static chime_object_t* BooleanEquality(chime_object_t* instance, const char* method_name, ...)
{
    va_list         arguments;
    chime_object_t* other;
    
    va_start(arguments, method_name);
    
    other = va_arg(arguments, chime_object_t*);
    
    va_end(arguments);
    
    // pointer equality test works in this case
    if (instance == other)
        return CHIME_LITERAL_TRUE;
    
    return CHIME_LITERAL_FALSE;
}

#pragma mark -
#pragma mark Literal Runtime Functions
void chime_literal_initialize(void)
{
    chime_object_t* new_class;
        
    new_class = chime_runtime_create_class("Null", _object_class);
    assert(new_class);
    
    chime_object_set_function(new_class, "print", NullPrint, 1);
    
    new_class = chime_runtime_create_class("Integer", _object_class);
    assert(new_class);
    
    chime_object_set_function(new_class, "print", IntegerPrint, 1);
    chime_object_set_function(new_class, "+",     IntegerAdd, 2);
    chime_object_set_function(new_class, "*",     IntegerMultiply, 2);
    chime_object_set_function(new_class, "<=>",   IntegerSpaceship, 2);
    chime_object_set_function(new_class, "==",    IntegerEquality, 2);
    
    new_class = chime_runtime_create_class("Boolean", _object_class);
    assert(new_class);
    
    chime_object_set_function(new_class, "print", BooleanPrint, 1);
    chime_object_set_function(new_class, "<=>",   BooleanSpaceship, 2);
    chime_object_set_function(new_class, "==",    BooleanEquality, 2);
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
