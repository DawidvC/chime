// Chime Runtime: chime_hash.c

#include "chime_hash.h"
#include "chime_hash_methods.h"
#include "runtime/class/chime_class.h"
#include "runtime/chime_runtime_internal.h"

void chime_hash_initialize(void)
{
    _hash_class = chime_class_create_object_subclass("Hash");
    
    // class methods
    chime_class_set_class_method(_hash_class, "new", hash_class_new);
    
    // instance methods
    chime_class_set_instance_method(_hash_class, "length", hash_length);
    chime_class_set_instance_method(_hash_class, "[]",     hash_indexer_get);
    chime_class_set_instance_method(_hash_class, "[]=",    hash_indexer_set);
}