// Chime Runtime: chime_reference.c

#include "chime_reference.h"
#include "runtime/support.h"
#include "runtime/chime_runtime.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/core/object/chime_object.h"
#include "runtime/core/object/chime_object_internal.h"
#include "runtime/core/class/chime_class.h"

#include "runtime/support.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//#define PRINT_REFERENCE

void chime_reference_initialize(void)
{
    _reference_class = chime_class_create_object_subclass("Reference");
}

chime_object_t* chime_reference_create(chime_object_t* object)
{
    chime_object_t* reference;
    
    assert(object);
    
    reference = chime_object_raw_create(_reference_class, sizeof(chime_object_t), false);
    reference->variables = NULL;
    
#ifdef PRINT_REFERENCE
    fprintf(stderr, "Creating a reference %p for %p\n", reference, object);
#endif
    
    chime_reference_set(reference, object);

#ifdef PRINT_REFERENCE
    fprintf(stderr, "Created a reference %p for %p\n", reference, object);
#endif
    
    return reference;
}

void chime_reference_destroy(chime_object_t* reference)
{
    assert(reference);
    
#ifdef PRINT_REFERENCE
    fprintf(stderr, "Destroying a reference %p for %p\n", reference, reference->variables);
#endif
    
    chime_dictionary_destroy(reference->methods);
    chime_object_release((chime_object_t*)reference->variables);
    
#ifdef PRINT_REFERENCE
    fprintf(stderr, "Destroyed a reference %p for %p\n", reference, reference->variables);
#endif
    
    free(reference);
}

chime_object_t* chime_reference_get(chime_object_t* reference)
{
    assert(reference);
    
    return (chime_object_t*)reference->variables;
}

void chime_reference_set(chime_object_t* reference, chime_object_t* object)
{
    assert(reference);
    
    chime_object_retain(object);
    
    chime_object_release((chime_object_t*)reference->variables);
    
    reference->variables = (chime_dictionary_t*)object;
}
