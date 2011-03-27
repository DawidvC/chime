// Chime Runtime: chime_object.c

#include "chime_object.h"
#include "chime_object_internal.h"
#include "chime_runtime.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

chime_object_t* chime_object_create(chime_object_t* object_class)
{
    chime_object_t* object;
    
    object = (chime_object_t*)malloc(sizeof(chime_object_t));
    
    object->flags           = 0;
    object->reference_count = 1;
    
    object->self_class  = object_class;
    
    if (object_class)
    {
        object->super_class = object_class->super_class;
    }
    else
    {
        object->super_class = 0;
    }    
    
    object->properties  = 0;
    
    return object;
}

chime_object_t* chime_object_create_with_name(const char* class_name)
{
    chime_object_t* object_class;
    
    object_class = chime_runtime_get_class(class_name);
    
    // class must not be null
    assert(object_class);
    
    return chime_object_create(object_class);
}

void chime_object_destroy(chime_object_t* object)
{
    assert(object);
    
    if (object->properties)
    {
        free(object->properties);
    }
    
    free(object);
}

chime_object_t* chime_object_get_property(chime_object_t* instance, const char* name)
{
    if (!instance->properties)
    {
        return 0;
    }
        
    return (chime_object_t*)chime_dictionary_get(instance->properties, name);
}

void chime_object_set_property(chime_object_t* instance, const char* name, chime_object_t* value)
{
    if (!instance->properties)
    {
        instance->properties = chime_dictionary_create();
    }
    
    chime_dictionary_set(instance->properties, name, value);
}

chime_function_t chime_object_get_function(chime_object_t* instance)
{
    return (chime_function_t)instance->properties;
}

void chime_object_set_function(chime_object_t* instance, const char* name, chime_function_t function)
{
    chime_object_t* method_object;
    
    method_object = chime_object_create_with_name("Method");
    method_object->properties = (void*)function;
    
    chime_object_set_property(instance, name, method_object);
}

chime_object_t* chime_object_invoke(chime_object_t* instance, const char* name, ...)
{
    chime_object_t*  method_object;
    chime_function_t function;
    
    method_object = chime_object_get_property(instance, name);
    function      = chime_object_get_function(method_object);
    
    return function(instance, name);
}
