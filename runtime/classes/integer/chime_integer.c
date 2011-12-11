#include "chime_integer.h"
#include "runtime/chime_runtime.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/core/class/chime_class.h"
#include "runtime/classes/string/chime_string.h"
#include "runtime/core/closure/chime_closure.h"
#include "runtime/tagging.h"
#include "runtime/platform.h"

#include <stdio.h>
#include <assert.h>

// a 64-bit integer can have 20 digits, plus a minus sign, plus a null
// terminator.  So we need at least 22 bytes.
#define BYTES_PER_INTEGER_STRING (22)
#define INTEGER_STRING_SIZE      ((BYTES_PER_INTEGER_STRING / CHIME_ALLOCATION_UNIT) + 1)

void chime_integer_initialize(void)
{
    chime_class_t* klass;
    
    klass = chime_class_create_object_subclass("Integer");
    
    chime_class_include_trait(klass, _comparable_trait);
    
    chime_class_set_instance_method(klass, "to_string", integer_to_string);
    chime_class_set_instance_method(klass, "times:",    integer_times);
    chime_class_set_instance_method(klass, "+",         integer_add);
    chime_class_set_instance_method(klass, "-",         integer_subtract);
    chime_class_set_instance_method(klass, "*",         integer_multiply);
    chime_class_set_instance_method(klass, "/",         integer_divide);
    chime_class_set_instance_method(klass, "%",         integer_modulus);
    chime_class_set_instance_method(klass, "<=>",       integer_compare);
}

chime_object_t* chime_integer_encode(signed long value)
{
    return chime_tag_encode_integer(value);
}
signed long chime_integer_decode(chime_object_t* object)
{
    return chime_tag_decode_integer(object);
}

chime_object_t* integer_to_string(chime_object_t* instance)
{
    chime_object_t* string;
    signed long     value;
    bool            has_sign;
    char*           c_string;
    char*           p;
    unsigned int    length;
    
    assert(chime_object_is_integer(instance));
    
    value = chime_integer_decode(instance);
    if (value == 0)
        return chime_string_create_with_c_string("0", 1);
    
    // the approach here, which is a little crazy, is to allocate a buffer
    // large enough for the biggest integer.  Then, we start filling in the
    // digits *backwards* using the modulus of the number.
    
    c_string = chime_allocate(INTEGER_STRING_SIZE);
    length   = 0;
    p        = c_string + BYTES_PER_INTEGER_STRING;
    
    if (value < 0)
    {
        value     = value * -1; // make it positive
        length   += 1;          // make space for the sign, which comes at the end
        has_sign  = true;
    }
    
    // null-terminate the string
    *p = 0;
    
    while ((value > 0) && (p >= c_string))
    {
        p -= 1;
        
        *p    = (value % 10) + 48; // convert digit to ASCII and store in the string
        value = value / 10;
        
        length += 1;
    }
    
    if (has_sign)
    {
        p -= 1;
        *p = '-'; // deal with the sign
    }
    
    string = chime_string_create_with_c_string(p, length);
    
    chime_deallocate(c_string); // remove our scratch space
    
    return string;
}

chime_object_t* integer_times(chime_object_t* instance, chime_object_t* function)
{
    signed long value;
    signed long i;
    
    assert(chime_object_is_integer(instance));
    
    value = chime_integer_decode(instance);
    if (value < 0)
    {
        fprintf(stderr, "[runtime] Integer#times doesn't work on negative integers\n");
        return CHIME_LITERAL_NULL;
    }
    
    // ok, so, what we need to is simply iterate on the block
    for (i = 0; i < value; ++i)
    {
        chime_closure_invoke((chime_closure_t*)function, chime_integer_encode(i));
    }
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* integer_add(chime_object_t* instance, chime_object_t* other)
{
    assert(chime_object_is_integer(instance));
    
    if (chime_object_is_integer(other))
    {
        signed long result;
        
        result = chime_integer_decode(instance) + chime_integer_decode(other);
        
        return chime_integer_encode(result);
    }
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* integer_subtract(chime_object_t* instance, chime_object_t* other)
{
    assert(chime_object_is_integer(instance));
    
    if (chime_object_is_integer(other))
    {
        signed long result;
        
        result = chime_integer_decode(instance) - chime_integer_decode(other);
        
        return chime_integer_encode(result);
    }
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* integer_multiply(chime_object_t* instance, chime_object_t* other)
{
    assert(chime_object_is_integer(instance));
    
    if (chime_object_is_integer(other))
    {
        signed long result;
        
        result = chime_integer_decode(instance) * chime_integer_decode(other);
        
        return chime_integer_encode(result);
    }
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* integer_divide(chime_object_t* instance, chime_object_t* other)
{
    if (chime_object_is_integer(other))
    {
        signed long result;
        
        result = chime_integer_decode(instance) / chime_integer_decode(other);
        
        return chime_integer_encode(result);
    }
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* integer_modulus(chime_object_t* instance, chime_object_t* other)
{
    if (chime_object_is_integer(other))
    {
        signed long result;
        
        result = chime_integer_decode(instance) % chime_integer_decode(other);
        
        return chime_integer_encode(result);
    }
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* integer_compare(chime_object_t* instance, chime_object_t* other)
{
    assert(chime_object_is_integer(instance) && "First argument to <=> must be an Integer");
    
    if (chime_object_is_integer(other))
    {
        signed long a_value;
        signed long b_value;
        
        a_value = chime_integer_decode(instance);
        b_value = chime_integer_decode(other);
        
        if (a_value == b_value)
            return chime_integer_encode(0);
        
        if (a_value < b_value)
            return chime_integer_encode(-1);
        
        if (a_value > b_value)
            return chime_integer_encode(1);
        
        assert(0 && "Integer <=> wasn't able to compare values");
    }
    
    if (chime_log_level >= 5)
        fprintf(stderr, "[runtime] comparison of integer with non-integer\n");
    
    // if it's not an integer, let's just return nil, meaning comparison is impossible
    return CHIME_LITERAL_NULL;
}
