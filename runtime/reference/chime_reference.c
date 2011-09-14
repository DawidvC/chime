// Chime Runtime: chime_reference.c

#include "chime_reference.h"
#include "runtime/chime_runtime.h"
#include "runtime/object/chime_object_internal.h"
#include "runtime/collections/chime_dictionary.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void chime_reference_initialize(void)
{
    chime_class_t* klass;
    
    klass = chime_class_create_object_subclass("Reference");
}

chime_object_t* chime_reference_create(chime_object_t* object)
{
    chime_object_t* reference;
    
    assert(object);
    
    reference = (chime_object_t*)malloc(sizeof(chime_object_t));
    
    reference->self_class = chime_runtime_get_class("Reference");
    reference->flags      = 0;
    reference->methods    = chime_dictionary_create();
    reference->variables  = (chime_dictionary_t*)object; // hijack this pointer
    
    //fprintf(stderr, "Created a reference %p for %p\n", reference, object);
    
    return reference;
}

void chime_reference_destroy(chime_object_t* reference)
{
    assert(reference);
    
    chime_dictionary_destroy(reference->methods);
    
    free(reference);
}

chime_object_t* chime_reference_get(chime_object_t* reference)
{
    assert(reference);
    
    //fprintf(stderr, "Getting reference %p\n", reference);
    
    return (chime_object_t*)reference->variables;
}

void chime_reference_set(chime_object_t* reference, chime_object_t* object)
{
    assert(reference);
    
    //fprintf(stderr, "Setting reference %p to %p\n", reference, object);
    
    reference->variables = (chime_dictionary_t*)object;
}
