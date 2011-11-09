// Chime Runtime: chime_float_methods.c

#include "chime_float.h"
#include "chime_float_methods.h"
#include "runtime/literals/chime_literal.h"
#include "runtime/object/chime_object_internal.h"
#include "runtime/chime_runtime_internal.h"

#include <stdio.h>
#include <assert.h>

double get_double_value(chime_object_t* instance)
{
    if (chime_object_is_integer(instance))
    {
        return chime_literal_decode_integer(instance);
    }
    else if (chime_object_get_class(instance) == _float_class)
    {
        return chime_float_get_value(instance);
    }
    
    assert(0 && "Cannot get double value for object type");
    
    return 0;
}

chime_object_t* float_compare(chime_object_t* instance, chime_object_t* other)
{
    double value;
    double other_value;
    
    value       = chime_float_get_value(instance);
    other_value = get_double_value(other);
    
    if (value == other_value)
        return chime_literal_encode_integer(0);
    
    if (value < other_value)
        return chime_literal_encode_integer(-1);
    
    if (value > other_value)
        return chime_literal_encode_integer(1);
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* float_add(chime_object_t* instance, chime_object_t* other)
{
    double value;
    double other_value;
    
    value       = chime_float_get_value(instance);
    other_value = get_double_value(other);
    
    return chime_float_create(value + other_value);
}

chime_object_t* float_subtract(chime_object_t* instance, chime_object_t* other)
{
    double value;
    double other_value;
    
    value       = chime_float_get_value(instance);
    other_value = get_double_value(other);
    
    return chime_float_create(value - other_value);
}

chime_object_t* float_divide(chime_object_t* instance, chime_object_t* other)
{
    double value;
    double other_value;
    
    value       = chime_float_get_value(instance);
    other_value = get_double_value(other);
    
    return chime_float_create(value / other_value);
}

chime_object_t* float_multiply(chime_object_t* instance, chime_object_t* other)
{
    double value;
    double other_value;
    
    value       = chime_float_get_value(instance);
    other_value = get_double_value(other);
    
    return chime_float_create(value * other_value);
}
