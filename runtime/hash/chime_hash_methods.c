// Chime Runtime: chime_hash_methods.c

#include "chime_hash_methods.h"
#include "runtime/literals/chime_literal.h"
#include "runtime/string/chime_string.h"
#include "runtime/collections/chime_dictionary.h"

#include <assert.h>
#include <stdio.h>

static chime_dictionary_t* hash_get_internal_dictionary(chime_object_t* instance);

chime_object_t* hash_class_new(chime_class_t* klass)
{
    chime_object_t*             instance;
    chime_dictionary_t* internal_dictionary;
    
    // first thing to do is invoke new on super
    instance = chime_object_create(klass);
    
    internal_dictionary = chime_dictionary_create();
    assert(internal_dictionary);
    
    chime_object_set_property(instance, "_internal_dictionary", (chime_object_t*)internal_dictionary);
    
    return instance;
}

chime_object_t* hash_length(chime_object_t* instance)
{
    chime_dictionary_t* internal_dictionary;
    
    internal_dictionary = hash_get_internal_dictionary(instance);
    
    return chime_literal_encode_integer(chime_dictionary_count(internal_dictionary));
}

chime_object_t* hash_indexer_get(chime_object_t* instance, chime_object_t* index)
{
    chime_dictionary_t* internal_dictionary;
    chime_object_t*             hashed_value;
    
    internal_dictionary = hash_get_internal_dictionary(instance);
    
    hashed_value = chime_object_invoke_0(index, "hash");
    
    return chime_dictionary_get(internal_dictionary, chime_string_to_c_string(hashed_value));
}

chime_object_t* hash_indexer_set(chime_object_t* instance, chime_object_t* index, chime_object_t* value)
{
    chime_dictionary_t* internal_dictionary;
    chime_object_t*             hashed_value;
    
    internal_dictionary = hash_get_internal_dictionary(instance);
    
    hashed_value = chime_object_invoke_0(index, "hash");
    
    chime_dictionary_set(internal_dictionary, chime_string_to_c_string(hashed_value), value);
    
    return CHIME_LITERAL_NULL;
}

static chime_dictionary_t* hash_get_internal_dictionary(chime_object_t* instance)
{
    return (chime_dictionary_t*)chime_object_get_property(instance, "_internal_dictionary");
}
