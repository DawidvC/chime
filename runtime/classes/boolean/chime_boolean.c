#include "chime_boolean.h"
#include "runtime/core/class/chime_class.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/tagging.h"
#include "runtime/classes/integer/chime_integer.h"

#include "runtime/classes/string/chime_string.h"

#include <assert.h>
#include <stdio.h>

void chime_boolean_initialize(void)
{
    chime_class_t* klass;
    
    klass = chime_class_create_object_subclass("Boolean");
    
    chime_class_include_trait(klass, _comparable_trait);
    
    chime_class_set_instance_method(klass, "to_string", boolean_to_string);
    chime_class_set_instance_method(klass, "<=>",       boolean_compare);
}

chime_object_t* boolean_to_string(chime_object_t* instance)
{
    assert(chime_object_is_boolean(instance));
    
    if (instance == CHIME_LITERAL_TRUE)
    {
        return chime_string_create_with_c_string("true", 4);
    }
    
    return chime_string_create_with_c_string("false", 5);
}

chime_object_t* boolean_compare(chime_object_t* instance, chime_object_t* other)
{
    assert(chime_object_is_boolean(instance) && "First argument to <=> must be a Boolean");
    
    if (chime_object_is_boolean(other))
    {
        if (instance == other)
            return chime_integer_encode(0);
        
        if (other == CHIME_LITERAL_TRUE)
            return chime_integer_encode(-1);
        else
            return chime_integer_encode(1);
    }
    
    assert(0 && "comparison of Boolean with non-Boolean");
    
    return CHIME_LITERAL_NULL;
}
