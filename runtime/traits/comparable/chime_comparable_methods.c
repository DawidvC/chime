// Chime Runtime: chime_comparable_methods.c

#include "chime_comparable_methods.h"
#include "runtime/classes/integer/chime_integer.h"
#include "runtime/core/object/chime_object.h"
#include "runtime/tagging.h"

long spaceship_compare(chime_object_t* instance, chime_object_t* other)
{
    chime_object_t* result;
    
    result = chime_object_invoke_1(instance, "<=>", other);
    
    if (!chime_object_is_integer(result))
        return -2;
    
    return chime_integer_decode(result);
}

chime_object_t* comparable_equals(chime_object_t* instance, chime_object_t* other)
{
    if (spaceship_compare(instance, other) == 0)
        return CHIME_LITERAL_TRUE;
    
    return CHIME_LITERAL_FALSE;
}

chime_object_t* comparable_less_than(chime_object_t* instance, chime_object_t* other)
{
    if (spaceship_compare(instance, other) == -1)
        return CHIME_LITERAL_TRUE;
    
    return CHIME_LITERAL_FALSE;
}

chime_object_t* comparable_less_or_equals(chime_object_t* instance, chime_object_t* other)
{
    long result;
    
    result = spaceship_compare(instance, other);
    if ((result == -1) || (result == 0))
        return CHIME_LITERAL_TRUE;
    
    return CHIME_LITERAL_FALSE;
}

chime_object_t* comparable_greater_than(chime_object_t* instance, chime_object_t* other)
{
    if (spaceship_compare(instance, other) == 1)
        return CHIME_LITERAL_TRUE;
    
    return CHIME_LITERAL_FALSE;
}

chime_object_t* comparable_greater_or_equals(chime_object_t* instance, chime_object_t* other)
{
    long result;
    
    result = spaceship_compare(instance, other);
    if ((result == 1) || (result == 0))
        return CHIME_LITERAL_TRUE;
    
    return CHIME_LITERAL_FALSE;
}
