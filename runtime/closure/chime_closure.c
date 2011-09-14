// Chime Runtime: chime_closure.c

#include "chime_closure.h"
#include "chime_closure_internal.h"
#include "runtime/chime_runtime.h"
#include "chime_closure_methods.h"

#include <stdlib.h>
#include <assert.h>

void chime_closure_initialize(void)
{
    chime_class_t* klass;
    
    klass = chime_class_create_object_subclass("Function");
    
    chime_class_set_instance_method(klass, "to_string", closure_to_string);
    chime_class_set_instance_method(klass, "call",      closure_call);
}

chime_closure_t* chime_closure_create(void* function)
{
    chime_closure_t* closure;
    
    assert(function);
    
    closure = (chime_closure_t*)malloc(sizeof(chime_closure_t));
    
    closure->object.self_class = chime_runtime_get_class("Function");
    closure->object.flags      = 0;
    closure->object.methods    = chime_dictionary_create();
    closure->object.variables  = chime_dictionary_create();
    closure->function          = function;
    
    return closure;
}

void chime_closure_destroy(chime_closure_t* closure)
{
    assert(closure);
    
    chime_dictionary_destroy(closure->object.methods);
    chime_dictionary_destroy(closure->object.variables);
    
    free(closure);
}

chime_object_t* chime_closure_invoke(chime_closure_t* closure, chime_object_t* argument_array)
{
    assert(closure);
    
    return ((chime_function_arg0_t)closure->function)((chime_object_t*)closure);
}
