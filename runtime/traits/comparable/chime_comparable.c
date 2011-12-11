// Chime Runtime: chime_comparable.c

#include "chime_comparable.h"
#include "chime_comparable_methods.h"
#include "runtime/core/class/chime_class.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/core/trait/chime_trait.h"

void chime_comparable_initialize(void)
{
    _comparable_trait = chime_trait_create("Comparable");
    
    // instance methods
    chime_class_set_instance_method(_comparable_trait, "==", comparable_equals);
    chime_class_set_instance_method(_comparable_trait, "<",  comparable_less_than);
    chime_class_set_instance_method(_comparable_trait, "<=", comparable_less_or_equals);
    chime_class_set_instance_method(_comparable_trait, ">",  comparable_greater_than);
    chime_class_set_instance_method(_comparable_trait, ">=", comparable_greater_or_equals);
}
