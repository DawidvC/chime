// Chime Runtime: chime_string.c

#include "chime_string.h"
#include "chime_string_methods.h"
#include "runtime/chime_runtime.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/class/chime_class.h"
#include "runtime/object/chime_object_internal.h"
#include "runtime/string/chime_string.h"

#include <stdio.h>
#include <assert.h>

void chime_string_initialize(void)
{
    _string_class = chime_class_create_object_subclass("String");
    
    chime_class_set_instance_method(_string_class, "to_string", string_to_string);
    chime_class_set_instance_method(_string_class, "print",     string_print);
    chime_class_set_instance_method(_string_class, "<=>",       string_compare);
}

void chime_string_destroy(void)
{
    chime_class_destroy(_string_class);
    _string_class = NULL;
}

chime_object_t* chime_string_create_with_c_string(const char* string)
{
    chime_object_t* object;
    
    assert(string);
    
    object = chime_runtime_instantiate(_string_class);
    object->flags = (unsigned long)string;
    
    assert(object);
    
    return object;
}

char* chime_string_to_c_string(chime_object_t* instance)
{
    assert(instance && "Trying to convert NULL to a c-string");
        
    return (char*)instance->flags;
}
