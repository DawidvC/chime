// Chime Runtime: chime_literal_methods.c

#include "chime_literal_methods.h"
#include "chime_literal.h"
#include "runtime/chime_runtime.h"
#include "runtime/string/chime_string.h"
#include "runtime/closure/chime_closure.h"

#include <stdio.h>
#include <assert.h>

#pragma mark -
#pragma mark Null Functions
chime_object_t* null_print(chime_object_t* instance)
{
    assert(instance == CHIME_LITERAL_NULL);
    
    fprintf(stdout, "<Null>\n");
    
    return CHIME_LITERAL_NULL;
}

#pragma mark -
#pragma mark Integer Functions
chime_object_t* integer_print(chime_object_t* instance)
{
    assert(chime_object_is_integer(instance));
    
    fprintf(stdout, "<Integer: %ld>\n", chime_literal_decode_integer(instance));
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* integer_times(chime_object_t* instance, chime_object_t* function)
{
    signed long value;
    signed long i;
    
    assert(chime_object_is_integer(instance));
    
    value = chime_literal_decode_integer(instance);
    if (value < 0)
    {
        fprintf(stderr, "[runtime] Integer#times doesn't work on negative integers\n");
        return CHIME_LITERAL_NULL;
    }
        
    // ok, so, what we need to is simply iterate on the block
    for (i = 0; i < value; ++i)
    {
        chime_closure_invoke((chime_closure_t*)function, CHIME_LITERAL_NULL);
    }
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* integer_add(chime_object_t* instance, chime_object_t* other)
{
    assert(chime_object_is_integer(instance));
    
    if (chime_object_is_integer(other))
    {
        signed long result;
        
        result = chime_literal_decode_integer(instance) + chime_literal_decode_integer(other);
        
        return chime_literal_encode_integer(result);
    }
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* integer_multiply(chime_object_t* instance, chime_object_t* other)
{
    assert(chime_object_is_integer(instance));
    
    if (chime_object_is_integer(other))
    {
        signed long result;
        
        result = chime_literal_decode_integer(instance) * chime_literal_decode_integer(other);
        
        return chime_literal_encode_integer(result);
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
        
        a_value = chime_literal_decode_integer(instance);
        b_value = chime_literal_decode_integer(other);
        
        if (a_value == b_value)
            return chime_literal_encode_integer(0);
        
        if (a_value < b_value)
            return chime_literal_encode_integer(-1);
        
        if (a_value > b_value)
            return chime_literal_encode_integer(1);
        
        assert(0 && "Integer <=> wasn't able to compare values");
    }
    
    if (chime_log_level >= 5)
        fprintf(stderr, "[runtime] comparison of integer with non-integer\n");
    
    // if it's not an integer, let's just return nil, meaning comparison is impossible
    return CHIME_LITERAL_NULL;
}

#pragma mark -
#pragma mark Boolean Function
chime_object_t* boolean_print(chime_object_t* instance)
{
    assert(chime_object_is_boolean(instance));
    
    fprintf(stdout, "<Boolean: %s>\n", instance == CHIME_LITERAL_TRUE ? "true" : "false");
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* boolean_compare(chime_object_t* instance, chime_object_t* other)
{
    assert(chime_object_is_boolean(instance) && "First argument to <=> must be a Boolean");
    
    if (chime_object_is_boolean(other))
    {
        if (instance == other)
            return chime_literal_encode_integer(0);
        
        if (other == CHIME_LITERAL_TRUE)
            return chime_literal_encode_integer(-1);
        else
            return chime_literal_encode_integer(1);
    }
    
    if (chime_log_level >= 5)
        fprintf(stderr, "[runtime] comparison of Boolean with non-Boolean\n");
    
    return CHIME_LITERAL_NULL;
}
