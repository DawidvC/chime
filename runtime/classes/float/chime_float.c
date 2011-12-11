// Chime Runtime: chime_float.c

#include "chime_float.h"
#include "runtime/core/object/chime_object.h"
#include "runtime/core/object/chime_object_internal.h"
#include "runtime/core/class/chime_class.h"
#include "runtime/chime_runtime.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/classes/integer/chime_integer.h"
#include "runtime/platform.h"
#include "runtime/support.h"
#include "runtime/tagging.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

double get_double_value(chime_object_t* instance);

void chime_float_initialize(void)
{
    _float_class = chime_class_create_object_subclass("Float");
    
    // traits
    chime_class_include_trait(_float_class, _comparable_trait);
    
    // instance methods
    chime_class_set_instance_method(_float_class, "<=>",  float_compare);
    chime_class_set_instance_method(_float_class, "+",    float_add);
    chime_class_set_instance_method(_float_class, "-",    float_subtract);
    chime_class_set_instance_method(_float_class, "/",    float_divide);
    chime_class_set_instance_method(_float_class, "*",    float_multiply);
}

chime_object_t* chime_float_create(double value)
{
    chime_object_t* number;
    
    number = chime_object_raw_create(_float_class, sizeof(chime_object_t), false);
    
    chime_float_set_value(number, value);
    
    return number;
}

void chime_float_destroy(chime_object_t* instance)
{
    assert(instance);
    
    chime_dictionary_destroy(instance->methods);
    
    free(instance);
}

void chime_float_set_value(chime_object_t* instance, double value)
{
    memcpy(&(instance->variables), &(value), sizeof(double));
}

double chime_float_get_value(chime_object_t* instance)
{
    double value;
    
    memcpy(&value, &(instance->variables), sizeof(double));
    
    return value;
}

double get_double_value(chime_object_t* instance)
{
    if (chime_object_is_integer(instance))
    {
        return chime_integer_decode(instance);
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
        return chime_integer_encode(0);
    
    if (value < other_value)
        return chime_integer_encode(-1);
    
    if (value > other_value)
        return chime_integer_encode(1);
    
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
