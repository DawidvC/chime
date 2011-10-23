// Chime Runtime: chime_trait.c

#include "chime_trait.h"
#include "runtime/class/chime_class.h"
#include "runtime/chime_runtime.h"
#include "runtime/chime_runtime_internal.h"

void chime_trait_initialize(void)
{
    _trait_class = chime_class_create_object_subclass("Trait");
}

chime_object_t* chime_trait_create(const char* name)
{
    return chime_runtime_create_class(name, (chime_object_t*)_trait_class);
}
