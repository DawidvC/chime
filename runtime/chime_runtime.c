// Chime Runtime: chime_runtime.c

#include "chime_runtime.h"
#include "chime_runtime_internal.h"
#include "collections/chime_array.h"
#include "chime_object_internal.h"
#include <assert.h>

static chime_dictionary_t* _chime_classes    = 0;
static chime_object_t*     _root_meta_class  = 0;

void chime_runtime_initialize(void)
{
    chime_object_t* object_class;
    
    _chime_classes = chime_dictionary_create();
    
    assert(_chime_classes);
    
    // create the root class and meta-classes
    _root_meta_class = chime_object_create(0);
    assert(_root_meta_class);
    
    object_class = chime_runtime_create_class("Object", 0);
    assert(object_class);
    
    assert(chime_runtime_create_class("Method", object_class));
    
    assert(chime_runtime_create_class("Null", object_class));
    
    assert(chime_runtime_create_class("Integer", object_class));
}

void chime_runtime_destroy(void)
{
    chime_object_destroy(_root_meta_class);
    chime_dictionary_destroy(_chime_classes);
}

chime_object_t* chime_runtime_create_class(const char* name, chime_object_t* super_class)
{
    chime_object_t* class_object;
    chime_object_t* meta_class_object;
    
    class_object = chime_object_create(super_class);
    
    if (super_class == 0)
    {
        assert(_root_meta_class);
        meta_class_object = _root_meta_class;
    }
    else
    {
        meta_class_object = chime_object_create(super_class->self_class);
    }
    
    class_object->self_class = meta_class_object;
    
    chime_dictionary_set(_chime_classes, name, class_object);
    
    return class_object;
}

chime_object_t* chime_runtime_get_class(const char* name)
{
    return chime_dictionary_get(_chime_classes, name);
}