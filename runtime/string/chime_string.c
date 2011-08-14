// Chime Runtime: chime_string.c

#include "chime_string.h"
#include "chime_string_methods.h"
#include "runtime/chime_runtime.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/object/chime_object_internal.h"
#include "runtime/string/chime_string.h"

#include <stdio.h>
#include <assert.h>

void chime_string_initialize(void)
{
    _string_class = chime_runtime_create_object_subclass("String");
    assert(_string_class);
    
    chime_object_set_function(_string_class, "to_string", string_to_string, 0);
    chime_object_set_function(_string_class, "print",     string_print,     0);
    chime_object_set_function(_string_class, "<=>",       string_compare,   1);
}

chime_object_t* chime_string_create_with_c_string(const char* string)
{
    chime_object_t* object;
    
    assert(_string_class);
    assert(string);
    
    object = chime_object_create(_string_class);
    object->flags = (unsigned long)string;
    
    return object;
}

char* chime_string_to_c_string(chime_object_t* instance)
{
    assert(instance && "Trying to convert NULL to a c-string");
        
    return (char*)instance->flags;
}
