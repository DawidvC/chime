// Chime Runtime: chime_hash_methods.c

#include "chime_range_methods.h"
#include "runtime/core/object/chime_object.h"

chime_object_t* range_start_get(chime_object_t* instance)
{
    return chime_object_get_attribute(instance, "_start");
}

chime_object_t* range_start_set(chime_object_t* instance, chime_object_t* start)
{
    chime_object_set_attribute(instance, "_start", start);
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* range_end_get(chime_object_t* instance)
{
    return chime_object_get_attribute(instance, "_end");
}

chime_object_t* range_end_set(chime_object_t* instance, chime_object_t* end)
{
    chime_object_set_attribute(instance, "_end", end);
    
    return CHIME_LITERAL_NULL;
}
