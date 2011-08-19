#include "chime_array.h"
#include "runtime/class/chime_class.h"

#include <assert.h>

void chime_array_initialize(void)
{
    chime_class_t* array_class;
    
    array_class = chime_class_create_object_subclass("Array");
}
