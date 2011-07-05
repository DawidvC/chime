// Chime Runtime: assertions.c

#include "library/testing/assertions.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/chime_literals.h"
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

static chime_object_t* AssertIsTrue(chime_object_t* instance, const char* method_name, ...)
{
    chime_object_t* a;
    va_list         arguments;
    
    va_start(arguments, method_name);
    
    a = va_arg(arguments, chime_object_t*);
    
    va_end(arguments);
    
    if (a == CHIME_LITERAL_NULL || a == CHIME_LITERAL_FALSE)
    {
        fprintf(stderr, "[Assert] is_true: failed\n");
        chime_object_invoke(instance, "increment_failure_count");
        
        chime_object_invoke(a, "print");
    }
    
    return CHIME_LITERAL_NULL;
}

static chime_object_t* AssertIsFalse(chime_object_t* instance, const char* method_name, ...)
{
    chime_object_t* a;
    va_list         arguments;
    
    va_start(arguments, method_name);
    
    a = va_arg(arguments, chime_object_t*);
    
    va_end(arguments);
    
    if (!(a == CHIME_LITERAL_NULL || a == CHIME_LITERAL_FALSE))
    {
        fprintf(stderr, "[Assert] is_false: failed\n");
        chime_object_invoke(instance, "increment_failure_count");
    }
    
    return CHIME_LITERAL_NULL;
}

static chime_object_t* AssertIsNull(chime_object_t* instance, const char* method_name, ...)
{
    chime_object_t* a;
    va_list         arguments;
    
    va_start(arguments, method_name);
    
    a = va_arg(arguments, chime_object_t*);
    
    va_end(arguments);
    
    if (a != CHIME_LITERAL_NULL)
    {
        fprintf(stderr, "[Assert] is_null: failed\n");
        chime_object_invoke(instance, "increment_failure_count");
    }
    
    return CHIME_LITERAL_NULL;
}

static chime_object_t* AssertEqualTo(chime_object_t* instance, const char* method_name, ...)
{
    va_list         arguments;
    chime_object_t* a;
    chime_object_t* b;
    
    va_start(arguments, method_name);
    
    a = va_arg(arguments, chime_object_t*);
    b = va_arg(arguments, chime_object_t*);
    
    va_end(arguments);
    
    if (chime_object_invoke(a, "<=>", b) != chime_literal_encode_integer(0))
    {
        fprintf(stderr, "[Assert] equal:to: failed\n");
        chime_object_invoke(instance, "increment_failure_count");
        
        chime_object_invoke(a, "print");
        chime_object_invoke(b, "print");
    }
    
    return CHIME_LITERAL_NULL;
}

static chime_object_t* GetFailures(chime_object_t* instance, const char* method_name, ...)
{
    return chime_object_get_property(instance, "failure_count");
}

static chime_object_t* IncrementFailureCount(chime_object_t* instance, const char* method_name, ...)
{
    chime_object_t* failure_count;
    
    failure_count = chime_object_get_property(instance, "failure_count");
    
    failure_count = chime_object_invoke(failure_count, "+", chime_literal_encode_integer(1));
    
    chime_object_set_property(instance, "failure_count", failure_count);
    
    return failure_count;
}

void chime_assertion_initialize(void)
{
    chime_object_t* assertion_class;
    chime_object_t* assertion_metaclass;
    
    assertion_class = chime_runtime_create_class("Assert", _object_class);
    assert(assertion_class);
    
    // class methods
    assertion_metaclass = chime_object_get_class(assertion_class);
    
    chime_object_set_function(assertion_metaclass, "is_true",  AssertIsTrue,  1);
    chime_object_set_function(assertion_metaclass, "is_false", AssertIsFalse, 1);
    chime_object_set_function(assertion_metaclass, "is_null",  AssertIsNull,  1);
    chime_object_set_function(assertion_metaclass, "equal",    AssertEqualTo, 2);
    
    chime_object_set_function(assertion_metaclass, "failures", GetFailures,   0);
    chime_object_set_function(assertion_metaclass, "increment_failure_count", IncrementFailureCount, 0);
    
    chime_object_set_property(assertion_class, "failure_count", chime_literal_encode_integer(0));
}
