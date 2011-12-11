// Chime Runtime: assertions.c

#include "assertions.h"
#include "runtime/classes/integer/chime_integer.h"

#include <stdio.h>
#include <assert.h>

static void print_object(chime_object_t* instance)
{
    chime_object_invoke_0(chime_object_invoke_0(instance, "to_string"), "print");
}

chime_object_t* assert_class_failure(chime_object_t* instance)
{
    fprintf(stderr, "[Assert] \e[31mfailure\e[0m\n");
    chime_object_invoke_0(instance, "increment_failure_count");
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* assert_class_is_true(chime_object_t* instance, chime_object_t* object)
{
    if (object == CHIME_LITERAL_NULL || object == CHIME_LITERAL_FALSE)
    {
        fprintf(stderr, "[Assert] is_true: \e[31mfailed\e[0m\n");
        chime_object_invoke_0(instance, "increment_failure_count");
        
        print_object(object);
    }
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* assert_class_is_false(chime_object_t* instance, chime_object_t* object)
{
    if (!(object == CHIME_LITERAL_NULL || object == CHIME_LITERAL_FALSE))
    {
        fprintf(stderr, "[Assert] is_false: \e[31mfailed\e[0m\n");
        chime_object_invoke_0(instance, "increment_failure_count");
        
        print_object(object);
    }
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* assert_class_is_null(chime_object_t* instance, chime_object_t* object)
{
    if (object != CHIME_LITERAL_NULL)
    {
        fprintf(stderr, "[Assert] is_null: \e[31mfailed\e[0m\n");
        chime_object_invoke_0(instance, "increment_failure_count");
        
        print_object(object);
    }
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* assert_class_equal_to(chime_object_t* instance, chime_object_t* a, chime_object_t* b)
{
    if (chime_object_invoke_1(a, "<=>", b) != chime_integer_encode(0))
    {
        fprintf(stderr, "[Assert] equal:to: \e[31mfailed\e[0m\n");
        chime_object_invoke_0(instance, "increment_failure_count");
        
        print_object(a);
        print_object(b);
    }
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* assert_class_failure_count(chime_object_t* instance)
{
    return chime_object_get_property(instance, "failure_count");
}

chime_object_t* assert_class_increment_failure_count(chime_object_t* instance)
{
    chime_object_t* failure_count;
    
    failure_count = chime_object_get_property(instance, "failure_count");
    
    failure_count = chime_object_invoke_1(failure_count, "+", chime_integer_encode(1));
    
    chime_object_set_attribute(instance, "failure_count", failure_count);
    
    return failure_count;
}

void chime_assertion_initialize(void)
{
    chime_class_t* assertion_class;
    
    assertion_class = chime_class_create_object_subclass("Assert");
    
    chime_class_set_class_method(assertion_class, "failure",   assert_class_failure);
    chime_class_set_class_method(assertion_class, "is_true:",  assert_class_is_true);
    chime_class_set_class_method(assertion_class, "is_false:", assert_class_is_false);
    chime_class_set_class_method(assertion_class, "is_nil:",   assert_class_is_null);
    chime_class_set_class_method(assertion_class, "equal:to:", assert_class_equal_to);
    
    chime_class_set_class_method(assertion_class, "failure_count",           assert_class_failure_count);
    chime_class_set_class_method(assertion_class, "increment_failure_count", assert_class_increment_failure_count);
    
    chime_object_set_attribute((chime_object_t*)assertion_class, "failure_count", chime_integer_encode(0));
}
