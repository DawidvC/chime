#include "chime_closure_methods.h"
#include "chime_closure.h"
#include "runtime/literals/chime_literal.h"
#include "runtime/string/chime_string.h"

#include <stdio.h>
#include <assert.h>

chime_object_t* closure_to_string(chime_object_t* instance)
{
    return chime_string_create_with_c_string("<Closure: some more details>");
}

chime_object_t* closure_call(chime_object_t* instance, chime_object_t* argument_array)
{
    assert(instance);
    
    fprintf(stderr, "Closure invoked\n");
    
    return chime_closure_invoke((chime_closure_t*)instance, argument_array);
}