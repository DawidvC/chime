// Chime Runtime: chime_string.c

#include "chime_string.h"
#include "chime_runtime.h"
#include "chime_runtime_internal.h"
#include <stdio.h>
#include <assert.h>

static chime_object_t* StringPrint(chime_object_t* instance, const char* method_name, ...);

void chime_string_initialize(void)
{
    _string_class = chime_runtime_create_class("String", _object_class);
    assert(_string_class);
    
    chime_object_set_function(_string_class, "print", StringPrint, 0);
}

chime_object_t* chime_string_create_with_c_string(const char* string)
{
    chime_object_t* object;
    
    object = chime_object_create(_string_class);
    object->flags = (unsigned long)string;
    
    //fprintf(stderr, "Creating string instance '%s' => %p\n", string, object);
    
    return object;
}

char* chime_string_to_c_string(chime_object_t* instance)
{
    return (char*)instance->flags;
}

#pragma mark -
#pragma mark String Methods

static chime_object_t* StringPrint(chime_object_t* instance, const char* method_name, ...)
{
    fprintf(stdout, "<String: %s>\n", chime_string_to_c_string(instance));
    
    return CHIME_LITERAL_NULL;
}