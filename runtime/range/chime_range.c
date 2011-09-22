// Chime Runtime: chime_range.c

#include "chime_range.h"
#include "chime_range_methods.h"
#include "runtime/class/chime_class.h"
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
