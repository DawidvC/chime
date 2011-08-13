// Chime Runtime: chime_string.c

#include "chime_string.h"
#include "chime_runtime.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/object/chime_object_internal.h"
#include <stdio.h>
#include <assert.h>

static chime_object_t* StringPrint(chime_object_t* instance, const char* method_name, ...);

void chime_string_initialize(void)
{
    _string_class = chime_runtime_create_object_subclass("String");
    assert(_string_class);
    
    chime_object_set_function(_string_class, "print", StringPrint, 0);
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

#pragma mark -
#pragma mark String Methods

static chime_object_t* StringPrint(chime_object_t* instance, const char* method_name, ...)
{
    fprintf(stdout, "<String: %s>\n", chime_string_to_c_string(instance));
    
    return CHIME_LITERAL_NULL;
}