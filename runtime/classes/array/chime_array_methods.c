// Chime Runtime: chime_array_methods.c

#include "chime_array.h"
#include "chime_array_methods.h"
#include "runtime/classes/integer/chime_integer.h"
#include "runtime/core/closure/chime_closure.h"
#include "runtime/chime_runtime.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/core/object/chime_object.h"
#include "runtime/core/object/chime_object_internal.h"
#include "runtime/support.h"

#include <assert.h>
#include <stdio.h>

static chime_runtime_array_t* array_get_internal_array(chime_object_t* instance);

chime_object_t* array_class_new(chime_class_t* klass)
{
    return chime_array_create();
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
    
    return chime_integer_encode(chime_runtime_array_count(internal_array));
}

chime_object_t* array_contains(chime_object_t* instance, chime_object_t* value)
{
    chime_runtime_array_t* internal_array;
    unsigned long          length;
    signed long            i;
    
    internal_array = array_get_internal_array(instance);
    
    length = chime_runtime_array_count(internal_array);
    for (i = 0; i < length; ++i)
    {
        chime_object_t* item;
        
        item = chime_runtime_array_get(internal_array, i);
        if (chime_object_invoke_1(value, "==", item) == CHIME_LITERAL_TRUE)
            return CHIME_LITERAL_TRUE;
    }
    
    return CHIME_LITERAL_FALSE;
}

chime_object_t* array_append(chime_object_t* instance, chime_object_t* object)
{
    chime_runtime_array_t* internal_array;
    
    internal_array = array_get_internal_array(instance);
    
    chime_runtime_array_add(internal_array, object);
    chime_object_retain(object);
    
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

chime_object_t* array_map(chime_object_t* instance, chime_object_t* function)
{
    chime_object_t*        new_array;
    chime_runtime_array_t* internal_array;
    unsigned long          length;
    signed long            i;
    
    internal_array = array_get_internal_array(instance);
    
    new_array = chime_runtime_instantiate(_array_class);
    
    length = chime_runtime_array_count(internal_array);
    for (i = 0; i < length; ++i)
    {
        chime_object_t* transformed_item;
        
        transformed_item = chime_closure_invoke((chime_closure_t*)function, chime_runtime_array_get(internal_array, i));
        
        chime_object_invoke_1(new_array, "append:", transformed_item);
    }
    
    return new_array;
}

chime_object_t* array_indexer_get(chime_object_t* instance, chime_object_t* index)
{
    chime_runtime_array_t* internal_array;
    chime_object_t*        object;
    unsigned long          i;
    
    internal_array = array_get_internal_array(instance);
    
    i = chime_integer_decode(index);
    
    object = chime_runtime_array_get(internal_array, i);
    
    chime_object_retain(object);
    
    return object;
}

static chime_runtime_array_t* array_get_internal_array(chime_object_t* instance)
{
    return (chime_runtime_array_t*)chime_object_get_attributes(instance);
}
