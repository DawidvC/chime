// Chime Runtime: chime_class_methods.c

#include "chime_class_methods.h"
#include "chime_class.h"
#include "runtime/chime_runtime.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/object/chime_object_internal.h"
#include "runtime/literals/chime_literal.h"
#include "runtime/string/chime_string.h"
#include "runtime/array/chime_array_methods.h"

#include <string.h>
#include <stdio.h>
#include <assert.h>

chime_object_t* class_new(chime_object_t* instance)
{
    chime_object_t* object;
    
    object = chime_object_create((chime_class_t*)instance);
    
    // we actually need to walk the inheritance chain and initialize all super classes too
    chime_object_invoke_0(object, "initialize");
    
    return object;
}

chime_object_t* class_name(chime_object_t* instance)
{
    chime_object_t* string;
    char*           name;
    
    name   = chime_runtime_get_class_name((chime_class_t*)instance);
    string = chime_string_create_with_c_string(name, strlen(name));
    
    return string;
}

chime_object_t* class_equals(chime_object_t* instance, chime_object_t* other)
{
    // Because we are comparing classes, equality is actually just
    // pointer equality
    
    if (instance == other)
        return chime_literal_encode_integer(0);
    
    return CHIME_LITERAL_NULL;
}

static void append_methods_to_array(chime_object_t* klass, chime_object_t* array)
{
    chime_dictionary_t*    methods;
    chime_runtime_array_t* method_names;
    int                    i;
    
    methods      = chime_object_get_methods(klass);
    method_names = chime_dictionary_get_keys(methods);
    
    for (i = 0; i < chime_dictionary_count(methods); ++i)
    {
        chime_object_t* method;
        char*           method_name;
        
        method_name = chime_runtime_array_get(method_names, i);
        
        method = chime_runtime_instantiate(_method_class);
        chime_object_set_attribute(method, "_name", chime_string_create_with_c_string(method_name, strlen(method_name)));
        
        array_append(array, method);
        chime_object_release(method);
    }
}

chime_object_t* class_methods(chime_object_t* klass)
{
    chime_object_t* array;
    
    // create the array
    array = chime_runtime_instantiate(_array_class);
    
    do
    {
        append_methods_to_array(klass, array);
        
        klass = (chime_object_t*)chime_class_get_superclass((chime_class_t*)klass);
    } while (klass);
    
    return array;
}
