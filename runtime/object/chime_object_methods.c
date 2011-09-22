// Chime Runtime: chime_object_methods.c

#include "chime_object_methods.h"
#include "chime_object.h"
#include "chime_object_internal.h"
#include "runtime/chime_runtime.h"
#include "runtime/string/chime_string.h"
#include "runtime/literals/chime_literal.h"
#include "runtime/class/chime_class_methods.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

chime_object_t* object_class(chime_object_t* instance)
{
    return (chime_object_t*)chime_object_get_class(instance);
}

chime_object_t* object_methods(chime_object_t* instance)
{
    return class_methods(object_class(instance));
}

chime_object_t* object_invoke(chime_object_t* instance, chime_object_t* method_name)
{
    char* method;
    
    assert(instance);
    assert(method_name);
    
    method = chime_string_to_c_string(method_name);
    assert(method);
    
    return chime_object_invoke_0(instance, method);
}

chime_object_t* object_to_string(chime_object_t* instance)
{
    int             string_length;
    char*           buffer;
    char*           class_name;
    chime_class_t*  klass;
    chime_object_t* string;
    
    klass      = chime_object_get_class(instance);
    class_name = chime_class_get_name(klass);
    
    // we have to add the size of the "<:0x12345678>" to the string
    string_length = strlen(class_name) + sizeof(void*) + 6;
    
    buffer = malloc(string_length + 1);
    bzero(buffer, string_length + 1);
    
    snprintf(buffer, string_length + 1, "<%s:%p>", class_name, instance);
    
    string = chime_string_create_with_c_string(buffer, string_length + 1);
    
    free(buffer);
    
    return string;
}

chime_object_t* object_equals(chime_object_t* instance, chime_object_t* other)
{
    chime_object_t* result;
    
    result = chime_object_invoke_1(instance, "<=>", other);
    
    if (!chime_object_is_integer(result))
        return CHIME_LITERAL_FALSE;
    
    if (chime_literal_decode_integer(result) == 0)
        return CHIME_LITERAL_TRUE;
    
    return CHIME_LITERAL_FALSE;
}

chime_object_t* object_greater_than(chime_object_t* instance, chime_object_t* other)
{
    chime_object_t* result;
    
    result = chime_object_invoke_1(instance, "<=>", other);
    
    if (!chime_object_is_integer(result))
        return CHIME_LITERAL_FALSE;
    
    if (chime_literal_decode_integer(result) == 1)
        return CHIME_LITERAL_TRUE;
    
    return CHIME_LITERAL_FALSE;
}

chime_object_t* method_name(chime_object_t* instance)
{
    return chime_object_get_property(instance, "_name");
}
