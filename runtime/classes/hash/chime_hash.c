// Chime Runtime: chime_hash.c

#include "chime_hash.h"
#include "chime_hash_methods.h"
#include "runtime/core/class/chime_class.h"
#include "runtime/chime_runtime.h"
#include "runtime/chime_runtime_internal.h"

void chime_hash_initialize(void)
{
    _hash_class = chime_class_create_object_subclass("Hash");
    
    // instance methods
    chime_class_set_instance_method(_hash_class, "length", hash_length);
    chime_class_set_instance_method(_hash_class, "count",  hash_length);
    chime_class_set_instance_method(_hash_class, "[]:",    hash_indexer_get);
    chime_class_set_instance_method(_hash_class, "[]:=:",  hash_indexer_set);
}

chime_object_t* chime_hash_create(void)
{
    return chime_runtime_instantiate(_hash_class);
}

chime_object_t* chime_hash_set(chime_object_t* instance, chime_object_t* key, chime_object_t* value)
{
    return hash_indexer_set(instance, key, value);
}
