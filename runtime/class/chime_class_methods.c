// Chime Runtime: chime_class_methods.c

#include "chime_class_methods.h"
#include "runtime/literals/chime_literal.h"

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