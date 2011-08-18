// Chime Runtime: chime_array_methods.c

#include "chime_array_methods.h"
#include "runtime/literals/chime_literal.h"
#include "runtime/collections/chime_runtime_array.h"

#include <assert.h>

chime_object_t* array_class_new(chime_class_t* klass)
{
    chime_object_t*        array_instance;
    chime_runtime_array_t* internal_array;
    
    // first thing to do is invoke new on super
    array_instance = chime_object_create(klass);
    internal_array = chime_runtime_array_create();
    
    assert(internal_array);
    
    chime_object_set_property(array_instance, "_internal_array", (chime_object_t*)internal_array);
    
    return array_instance;
}

chime_object_t* array_to_string(chime_object_t* instance)
{
    return CHIME_LITERAL_NULL;
}

chime_object_t* array_length(chime_object_t* instance)
{
    chime_runtime_array_t* internal_array;
    
    internal_array = (chime_runtime_array_t*)chime_object_get_property(instance, "_internal_array");
    
    assert(internal_array);
    
    return chime_literal_encode_integer(chime_runtime_array_count(internal_array));
}

chime_object_t* array_add(chime_object_t* instance, chime_object_t* object)
{
    chime_runtime_array_t* internal_array;
    
    internal_array = (chime_runtime_array_t*)chime_object_get_property(instance, "_internal_array");
    
    chime_runtime_array_add(internal_array, object);
    
    return CHIME_LITERAL_NULL;
}
