// Chime Runtime: chime_float_methods.c

#include "chime_float.h"
#include "chime_float_methods.h"
#include "runtime/literals/chime_literal.h"
#include "runtime/object/chime_object_internal.h"
#include "runtime/chime_runtime_internal.h"

#include <stdio.h>
#include <assert.h>

chime_object_t* float_compare(chime_object_t* instance, chime_object_t* other)
{
    double value;
    double other_value;
    
    value = chime_float_get_value(instance);
    
    if (chime_object_is_integer(other))
    {
        other_value = chime_literal_decode_integer(other);
    }
    else if (chime_object_get_class(other) == _float_class)
    {
        other_value = chime_float_get_value(other);
    }
    else
    {
        assert(0 && "Float <=> wasn't able to compare values");
        
        return CHIME_LITERAL_NULL;
    }
    
    if (value == other_value)
        return chime_literal_encode_integer(0);
    
    if (value < other_value)
        return chime_literal_encode_integer(-1);
    
    if (value > other_value)
        return chime_literal_encode_integer(1);
    
    return CHIME_LITERAL_NULL;
}
