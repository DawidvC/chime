// Chime Runtime: chime_trait.c

#include "chime_trait.h"
#include "runtime/class/chime_class.h"
#include "runtime/class/chime_class_internal.h"
#include "runtime/chime_runtime.h"
#include "runtime/chime_runtime_internal.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void chime_trait_initialize(void)
{
    _trait_class = chime_class_create_object_subclass("Trait");
}

chime_class_t* chime_trait_create(const char* name)
{
    chime_class_t*  trait;
    chime_class_t*  metaclass;
    
    // create the metaclass first
    metaclass = (chime_class_t*)malloc(sizeof(chime_class_t));
    
    // entity portion
    metaclass->object.methods    = chime_dictionary_create(NULL);
    metaclass->object.self_class = NULL;
    metaclass->object.variables  = NULL; // metaclasses do not have variables
    
    // class portion
    assert(_trait_class);
    metaclass->super_class = _trait_class->object.self_class;
    metaclass->traits      = NULL;
    
    assert(metaclass);
    
    // now, we can create the actual class
    trait = (chime_class_t*)malloc(sizeof(chime_class_t));
    
    // entity portion
    trait->object.methods    = chime_dictionary_create(NULL);
    trait->object.self_class = metaclass;
    trait->object.variables  = chime_dictionary_create((chime_collection_finalizer)chime_object_release);
    
    // class portion
    trait->super_class = _trait_class;
    trait->traits      = NULL;
    
    assert(trait);
    
    chime_dictionary_set(_chime_traits, name, trait);
    
    return trait;
}
