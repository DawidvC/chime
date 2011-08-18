// Chime Runtime: chime_class_methods.c

#include "chime_class_methods.h"
#include "chime_class.h"
#include "runtime/chime_runtime.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/literals/chime_literal.h"

#include <assert.h>

chime_object_t* class_new(chime_object_t* instance)
{
    return chime_object_create((chime_class_t*)instance);
}

chime_object_t* class_name(chime_object_t* instance)
{
    return chime_object_get_property(instance, "_name");
}

chime_object_t* class_compare(chime_object_t* instance, chime_object_t* other)
{
    // Because we are comparing classes, equality is actually just
    // pointer equality.  The semantics of less-than and greater-than are
    // a little more complex.  For now, let's just punt.
    
    if (instance == other)
        return chime_literal_encode_integer(0);
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* class_methods(chime_object_t* instance)
{
    chime_object_t* array;
    
    array = chime_runtime_instantiate(_array_class);
    assert(array);
    
    
    
    return array;
}
