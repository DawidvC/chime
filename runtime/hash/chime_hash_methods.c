// Chime Runtime: chime_hash_methods.c

#include "chime_hash_methods.h"
#include "runtime/literals/chime_literal.h"
#include "runtime/string/chime_string.h"
#include "runtime/collections/chime_dictionary.h"

#include <assert.h>
#include <stdio.h>

chime_object_t* hash_class_new(chime_class_t* klass)
{
    chime_object_t* instance;
    
    // first thing to do is invoke new on super
    instance = chime_object_create(klass);
    
    return instance;
}

chime_object_t* hash_length(chime_object_t* instance)
{
    chime_dictionary_t* attributes;
    
    attributes = chime_object_get_attributes(instance);
    
    return chime_literal_encode_integer(chime_dictionary_count(attributes));
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
