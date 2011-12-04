// Chime Runtime: chime_reference.c

#include "chime_reference.h"
#include "runtime/support.h"
#include "runtime/chime_runtime.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/object/chime_object_internal.h"
#include "runtime/collections/chime_dictionary.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void chime_reference_initialize(void)
{
    _reference_class = chime_class_create_object_subclass("Reference");
}

chime_object_t* chime_reference_create(chime_object_t* object)
{
    chime_object_t* reference;
    
    assert(object);
    
    reference = chime_object_raw_create(_reference_class, sizeof(chime_object_t), false);
    
    chime_reference_set(reference, object);
    //fprintf(stderr, "Created a reference %p for %p\n", reference, object);
    
    return reference;
}

void chime_reference_destroy(chime_object_t* reference)
{
    assert(reference);
    
    chime_dictionary_destroy(reference->methods);
    chime_object_release((chime_object_t*)reference->variables);
    
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
    
    chime_object_retain(object);
    
    //fprintf(stderr, "Setting reference %p to %p\n", reference, object);
    chime_object_release((chime_object_t*)reference->variables);
    
    reference->variables = (chime_dictionary_t*)object;
}
