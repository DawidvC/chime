// Chime Runtime: chime_closure.c

#include "chime_closure.h"
#include "chime_closure_internal.h"
#include "runtime/chime_runtime.h"
#include "runtime/chime_runtime_internal.h"
#include "chime_closure_methods.h"
#include "runtime/core/class/chime_class.h"
#include "runtime/core/object/chime_object.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// #define PRINT_CLOSURES

void chime_closure_initialize(void)
{
    _closure_class = chime_class_create_object_subclass("Function");
    
    chime_class_set_instance_method(_closure_class, "to_string", closure_to_string);
    chime_class_set_instance_method(_closure_class, "call",      closure_call);
    chime_class_set_instance_method(_closure_class, "call:",     closure_call);
}

chime_closure_t* chime_closure_create(void* function)
{
    chime_closure_t* closure;
    
    assert(function);
    
    closure = (chime_closure_t*)malloc(sizeof(chime_closure_t));
    
    closure->object.self_class   = _closure_class;
    closure->object.flags        = 0;
    closure->object.retain_count = 1;
    closure->object.methods      = chime_dictionary_create(NULL);
    closure->object.variables    = chime_dictionary_create((chime_collection_finalizer)chime_object_release);
    closure->function            = function;
    
    return closure;
}

void chime_closure_destroy(chime_closure_t* closure)
{
    assert(closure);
    
#ifdef PRINT_CLOSURES
    fprintf(stderr, "Destroying a closure %p\n", closure);
#endif
    
    chime_dictionary_destroy(closure->object.methods);
    chime_dictionary_destroy(closure->object.variables);
    
    free(closure);
}

chime_object_t* chime_closure_invoke(chime_closure_t* closure, chime_object_t* arg1)
{
    assert(closure);
    
    return ((chime_function_arg1_t)closure->function)((chime_object_t*)closure, arg1);
}
