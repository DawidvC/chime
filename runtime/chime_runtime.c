// Chime Runtime: chime_runtime.c

#include "runtime/chime_runtime.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/collections/chime_array.h"
#include "runtime/chime_object_internal.h"
#include <assert.h>
#include <stdio.h>

static chime_dictionary_t* _chime_classes    = 0;
static chime_object_t*     _root_meta_class  = 0;
chime_object_t*            _object_class     = 0;
chime_object_t*            _string_class     = 0;
chime_object_t*            _method_class     = 0;
unsigned char              chime_log_level   = 4;

void chime_runtime_initialize(void)
{
    unsigned char old_level;
    
    // save original logging level
    old_level = chime_log_level;
    chime_log_level = 3;
    
    _chime_classes = chime_dictionary_create();
    
    assert(_chime_classes);
    
    // create the root class and meta-classes
    _root_meta_class = chime_object_create(0);
    assert(_root_meta_class);
    
    _object_class = chime_runtime_create_class("Object", 0);
    assert(_object_class);
    
    _method_class = chime_runtime_create_class("Method", _object_class);
    assert(_method_class);
    
    // once the root objects are created, strings come next,
    // because they tie into lots of stuff
    chime_string_initialize();
    
    // now go back and fix up the first classes, as they are missing the name properties
    chime_object_set_property(_root_meta_class, "name", chime_string_create_with_c_string("MetaClass"));
    chime_object_set_property(_object_class,    "name", chime_string_create_with_c_string("Object"));
    chime_object_set_property(_string_class,    "name", chime_string_create_with_c_string("String"));
    chime_object_set_property(_method_class,    "name", chime_string_create_with_c_string("Method"));
    
    chime_literal_initialize();
    
    assert(chime_runtime_create_class("External", _object_class));
    assert(chime_runtime_create_class("Main", _object_class));
    
    chime_log_level = old_level; // restore logging after initialization
}

void chime_runtime_destroy(void)
{
    chime_object_destroy(_method_class);
    _method_class = NULL;
    
    chime_object_destroy(_string_class);
    _string_class = NULL;
    
    chime_object_destroy(_object_class);
    _object_class = NULL;
    
    chime_object_destroy(_root_meta_class);
    _root_meta_class = NULL;
    
    chime_dictionary_destroy(_chime_classes);
    _chime_classes = NULL;
}

chime_object_t* chime_runtime_create_class(const char* name, chime_object_t* super_class)
{
    chime_object_t* class_object;
    chime_object_t* meta_class_object;
    
    if (chime_log_level >= 5)
        fprintf(stderr, "[runtime] Creating class '%s'\n", name);
    
    assert(!chime_object_is_literal(super_class) && "Cannot create a subclass of a literal");
    
    class_object = chime_object_create(super_class);
    
    if (_string_class)
    {
        // add a property on the class
        chime_object_t* string_object;
        
        string_object = chime_string_create_with_c_string(name);
        chime_object_set_property(class_object, "name", string_object);
    }
    
    if (super_class == 0)
    {
        assert(_root_meta_class && !_object_class);
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
    chime_object_t* object;
    
    object = chime_dictionary_get(_chime_classes, name);
    
    if (chime_log_level >= 5)
        fprintf(stderr, "[runtime] Getting class '%s' => %p\n", name, object);
    
    if (chime_log_level >= 4 && object == CHIME_LITERAL_NULL)
        fprintf(stderr, "[runtime] Getting class '%s' returned null\n", name);
    
    return object;
}
