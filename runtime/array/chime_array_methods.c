// Chime Runtime: chime_array_methods.c

#include "chime_array_methods.h"
#include "runtime/literals/chime_literal.h"
#include "runtime/closure/chime_closure.h"
#include "runtime/collections/chime_runtime_array.h"

#include <assert.h>

static chime_runtime_array_t* array_get_internal_array(chime_object_t* instance);

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
    
    internal_array = array_get_internal_array(instance);
    
    assert(internal_array);
    
    return chime_literal_encode_integer(chime_runtime_array_count(internal_array));
}

chime_object_t* array_append(chime_object_t* instance, chime_object_t* object)
{
    chime_runtime_array_t* internal_array;
    
    internal_array = array_get_internal_array(instance);
    
    chime_runtime_array_add(internal_array, object);
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* array_each(chime_object_t* instance, chime_object_t* function)
{
    chime_runtime_array_t* internal_array;
    unsigned long          length;
    signed long            i;
    
    internal_array = array_get_internal_array(instance);
    
    length = chime_runtime_array_count(internal_array);
    for (i = 0; i < length; ++i)
    {
        chime_closure_invoke((chime_closure_t*)function, chime_runtime_array_get(internal_array, i));
    }
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* array_indexer_get(chime_object_t* instance, chime_object_t* index)
{
    chime_runtime_array_t* internal_array;
    unsigned long          i;
    
    internal_array = array_get_internal_array(instance);
    
    i = chime_literal_decode_integer(index);
    
    return chime_runtime_array_get(internal_array, i);
}

static chime_runtime_array_t* array_get_internal_array(chime_object_t* instance)
{
    return (chime_runtime_array_t*)chime_object_get_property(instance, "_internal_array");
}
