// Chime Runtime: chime_hash_methods.c

#include "chime_hash_methods.h"
#include "runtime/classes/integer/chime_integer.h"
#include "runtime/classes/string/chime_string.h"
#include "runtime/core/object/chime_object.h"
#include "runtime/support.h"

#include <assert.h>
#include <stdio.h>

chime_object_t* hash_length(chime_object_t* instance)
{
    chime_dictionary_t* attributes;
    
    attributes = chime_object_get_attributes(instance);
    
    return chime_integer_encode(chime_dictionary_count(attributes));
}

chime_object_t* hash_indexer_get(chime_object_t* instance, chime_object_t* index)
{
    chime_object_t* hashed_value;
    chime_object_t* value;
    
    hashed_value = chime_object_invoke_0(index, "hash");
    value        = chime_object_get_attribute(instance, chime_string_to_c_string(hashed_value));
    
    chime_object_release(hashed_value);
    
    return value;
}

chime_object_t* hash_indexer_set(chime_object_t* instance, chime_object_t* index, chime_object_t* value)
{
    chime_object_t* hashed_value;
    
    hashed_value = chime_object_invoke_0(index, "hash");
    
    chime_object_set_attribute(instance, chime_string_to_c_string(hashed_value), value);
    
    chime_object_release(hashed_value);
    
    return CHIME_LITERAL_NULL;
}
