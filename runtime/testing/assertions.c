// Chime Runtime: assertions.c

#include "runtime/testing/assertions.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/chime_literals.h"
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

static chime_object_t* AssertTrue(chime_object_t* instance, const char* method_name, ...)
{
    chime_object_t* a;
    va_list         arguments;
    
    va_start(arguments, method_name);
    
    a = va_arg(arguments, chime_object_t*);
    
    va_end(arguments);
    
    assert(a == CHIME_LITERAL_TRUE);
    
    return CHIME_LITERAL_NULL;
}

void chime_assertion_initialize(void)
{
    chime_object_t* assertion_class;
    
    assertion_class = chime_runtime_create_class("Assert", _object_class);
    assert(assertion_class);
    
    chime_object_set_function(assertion_class, "true", AssertTrue, 2);
}

chime_object_t* chime_string_create_with_c_string(const char* string)
{
    chime_object_t* object;
    
    object = chime_object_create(_string_class);
    object->flags = (unsigned long)string;
    
    fprintf(stderr, "Creating string instance '%s' => %p\n", string, object);
    
    return object;
}
