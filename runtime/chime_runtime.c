// Chime Runtime: chime_runtime.c

#include "runtime/chime_runtime.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/collections/chime_array.h"
#include "runtime/chime_object_internal.h"
#include <assert.h>
#include <stdio.h>

static chime_dictionary_t* _chime_classes   = 0;
static chime_object_t*     _root_metaclass  = 0;
chime_object_t*            _class_class     = 0;
chime_object_t*            _object_class    = 0;
chime_object_t*            _string_class    = 0;
chime_object_t*            _method_class    = 0;
chime_object_t*            _undefined_class = 0;
unsigned char              chime_log_level  = 4;

void chime_runtime_initialize(void)
{
    unsigned char old_level;
    
    // save original logging level
    old_level = chime_log_level;
    chime_log_level = 3;
    
    _chime_classes = chime_dictionary_create();
    assert(_chime_classes);
    
    _class_class = chime_object_create(0);
    assert(_class_class);
    
    _class_class->self_class = _class_class;
    
    // create the root metaclass, which is needed to call
    // the chime_runtime_create_class function
    _root_metaclass = chime_object_create(0);
    assert(_root_metaclass);
    
    // create the root class
    _object_class = chime_runtime_create_class("Object", 0);
    assert(_object_class);
    
    _method_class = chime_runtime_create_class("Method", _object_class);
    assert(_method_class);
    
    _undefined_class = chime_runtime_create_class("Undefined", _object_class);
    assert(_undefined_class);
    
    // once the root objects are created, we can finish up with the object
    // class
    chime_object_initialize();
    
    // strings come next, because they tie into lots of stuff
    chime_string_initialize();
    
    // now go back and fix up the first classes, as they are missing the name properties
    chime_object_set_property(_root_metaclass,  "name", chime_string_create_with_c_string("MetaClass"));
    chime_object_set_property(_object_class,    "name", chime_string_create_with_c_string("Object"));
    chime_object_set_property(_string_class,    "name", chime_string_create_with_c_string("String"));
    chime_object_set_property(_method_class,    "name", chime_string_create_with_c_string("Method"));
    chime_object_set_property(_undefined_class, "name", chime_string_create_with_c_string("Undefined"));
    
    chime_literal_initialize();
    
    assert(chime_runtime_create_class("External", _object_class));
    assert(chime_runtime_create_class("Main", _object_class));
    
    chime_log_level = old_level; // restore logging after initialization
}

void chime_runtime_destroy(void)
{
    chime_object_destroy(_root_metaclass);
    _root_metaclass = NULL;
    
    // this should also remove all of the contained objects
    chime_dictionary_destroy(_chime_classes);
    _chime_classes = NULL;
    
    _object_class    = NULL;
    _string_class    = NULL;
    _method_class    = NULL;
    _undefined_class = NULL;
}

chime_object_t* chime_runtime_create_class(const char* name, chime_object_t* superclass)
{
    chime_object_t* class_object;
    chime_object_t* metaclass_object;
    
    if (chime_log_level >= 5)
        fprintf(stderr, "[runtime] Creating class '%s'\n", name);
    
    // create the meta-class
    if (superclass == 0)
    {
        // this is only allowed in the very special case that we've not yet established
        // the root classes yet
        assert(_root_metaclass && !_object_class && "With a null superclass, the root cases should all be uninitialized");
        metaclass_object = _root_metaclass;
        
        chime_object_set_property(_root_metaclass, "name", chime_string_create_with_c_string("MetaObject"));
    }
    else
    {
        assert(!chime_object_is_literal(superclass) && "Cannot create a subclass of a literal");
        
        metaclass_object = chime_object_create(0);
        metaclass_object->super_class = superclass->self_class;
        
        if (_string_class)
        {
            // This doesn't really work, because the name should really be "Meta" + name
            chime_object_set_property(metaclass_object, "name", chime_string_create_with_c_string("MetaSomething"));
        }
    }
    
    // now that we have the metaclass, create the new class, which is an instance of
    // the metaclass
    class_object = chime_object_create(metaclass_object);
    
    // chime_object_create makes the assumping it is creating a regular instance, so
    // we need to adjust the super_class here
    class_object->super_class = superclass;
    
    if (_string_class)
    {
        // add a property on the class
        chime_object_t* string_object;
        
        string_object = chime_string_create_with_c_string(name);
        chime_object_set_property(class_object, "name", string_object);
    }
    
    // store it in our class dictionary so we can look it up later by name
    chime_dictionary_set(_chime_classes, name, class_object);
    
    return class_object;
}

chime_object_t* chime_runtime_get_class(const char* name)
{
    chime_object_t* object;
    
    object = chime_dictionary_get(_chime_classes, name);
    
    if (chime_log_level >= 5)
        fprintf(stderr, "[runtime] Getting class '%s' => '%s'\n", name, chime_runtime_get_class_name(object));
    
    if (chime_log_level >= 4 && object == CHIME_LITERAL_NULL)
    {
        fprintf(stderr, "[runtime] Getting class '%s' returned null\n", name);
        object = _undefined_class;
    }
    
    return object;
}

char* chime_runtime_get_class_name(chime_object_t* instance)
{
    chime_object_t* name;
    
    assert(instance);
    
    name = chime_object_get_property(instance, "name");
    
    assert(name);
    
    return chime_string_to_c_string(name);
}

chime_object_t* chime_runtime_load(const char* name)
{
    fprintf(stderr, "Asked to load '%s'", name);
    
    return CHIME_LITERAL_TRUE;
}
