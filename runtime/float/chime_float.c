// Chime Runtime: chime_float.c

#include "chime_float.h"
#include "chime_float_methods.h"
#include "runtime/support.h"
#include "runtime/object/chime_object_internal.h"
#include "runtime/collections/chime_dictionary.h"
#include "runtime/class/chime_class.h"
#include "runtime/chime_runtime.h"
#include "runtime/chime_runtime_internal.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

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
