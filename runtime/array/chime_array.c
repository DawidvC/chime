// Chime Runtime: chime_array.c

#include "chime_array.h"
#include "chime_array_methods.h"
#include "runtime/class/chime_class.h"
#include "runtime/chime_runtime.h"
#include "runtime/chime_runtime_internal.h"

void chime_array_initialize(void)
{
    _array_class = chime_class_create_object_subclass("Array");
    
    // class methods
    chime_class_set_class_method(_array_class, "new", array_class_new);
    
    // instance methods
    chime_class_set_instance_method(_array_class, "length", array_length);
    chime_class_set_instance_method(_array_class, "count",  array_length);
    chime_class_set_instance_method(_array_class, "append", array_append);
    chime_class_set_instance_method(_array_class, "each",   array_each);
    chime_class_set_instance_method(_array_class, "[]",     array_indexer_get);
}

chime_object_t* chime_array_create(void)
{
    return chime_runtime_instantiate(_array_class);
}

chime_object_t* chime_array_create_with_length(unsigned long initial_length)
{
#pragma unused(initial_length)

    return chime_array_create();
}

chime_object_t* chime_array_append(chime_object_t* instance, chime_object_t* object)
{
    return array_append(instance, object);
}
