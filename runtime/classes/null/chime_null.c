#include "chime_null.h"
#include "runtime/core/class/chime_class.h"

#include "runtime/classes/string/chime_string.h"

void chime_null_initialize(void)
{
    chime_class_t* klass;
    
    klass = chime_class_create_object_subclass("Null");
    
    chime_class_set_instance_method(klass, "to_string", null_to_string);
}

chime_object_t* null_to_string(chime_object_t* instance)
{
    return chime_string_create_with_c_string("null", 4);
}
