// Chime Runtime: chime_range.c

#include "chime_range.h"
#include "chime_range_methods.h"
#include "runtime/core/class/chime_class.h"
#include "runtime/core/object/chime_object.h"
#include "runtime/chime_runtime_internal.h"

void chime_range_initialize(void)
{
    _range_class = chime_class_create_object_subclass("Range");
    
    // instance methods
    chime_class_set_instance_method(_range_class, "start",  range_start_get);
    chime_class_set_instance_method(_range_class, "start=", range_start_set);
    chime_class_set_instance_method(_range_class, "end",    range_end_get);
    chime_class_set_instance_method(_range_class, "end=",   range_end_set);
}

chime_object_t* chime_range_create(chime_object_t* start, chime_object_t* end)
{
    chime_object_t* range;
    
    range = chime_class_instantiate(_range_class);
    
    chime_object_invoke_1(range, "start=", start);
    chime_object_invoke_1(range, "end=", end);
    
    return range;
}