// Chime Runtime: chime_class.c

#include "chime_class.h"
#include "chime_class_internal.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/chime_runtime.h"

#include <stdlib.h>
#include <assert.h>

chime_class_t*  chime_class_create_object_subclass(const char* name)
{
    return chime_class_create(name, _object_class);
}

chime_class_t* chime_class_create(const char* name, chime_class_t* superclass)
{
    chime_class_t*  klass;
    chime_class_t*  metaclass;
    
    // create the metaclass first
    metaclass = (chime_class_t*)malloc(sizeof(chime_class_t));
    
    // entity portion
    metaclass->object.methods    = chime_dictionary_create();
    metaclass->object.self_class = NULL;
    
    // class portion
    if (superclass)
    {
        metaclass->super_class = superclass->object.self_class;
    }
    else
    {
        // this is only allowed in the very special case that we've not yet established
        // the root classes yet
        assert(!_object_class && "With a null superclass, the root classes should all be uninitialized");
        metaclass->super_class = NULL;
    }
    
    assert(metaclass);
    
    metaclass->object.variables = NULL; // metaclasses do not have variables
    
    // now, we can create the actual class
    klass = (chime_class_t*)malloc(sizeof(chime_class_t));
    
    // entity portion
    klass->object.methods    = chime_dictionary_create();
    klass->object.self_class = metaclass;
    
    // class portion
    klass->super_class      = superclass;
    klass->object.variables = chime_dictionary_create();
    
    assert(klass);
    
    chime_dictionary_set(_chime_classes, name, klass);
    
    return klass;
}

void chime_class_destroy(chime_class_t* klass)
{
    assert(klass);
    
    // should remove from _chime_classes here, but we don't know the key
    
    chime_dictionary_destroy(klass->object.methods);
    chime_dictionary_destroy(klass->object.variables);
    
    free(klass);
}

chime_class_t* chime_class_get_class(chime_class_t* klass)
{
    assert(klass);
    
    return klass->object.self_class;
}

chime_class_t* chime_class_get_superclass(chime_class_t* klass)
{
    assert(klass);
    
    return klass->super_class;
}

char* chime_class_get_name(chime_class_t* klass)
{
    return chime_runtime_get_class_name(klass);
}

void chime_class_set_instance_method(chime_class_t* klass, const char* name, void* function)
{
    assert(function);
    assert(name);
    
    chime_dictionary_set(klass->object.methods, name, function);
}

void chime_class_set_class_method(chime_class_t* klass, const char* name, void* function)
{
    // a class method is really an instance method registered to the meta class
    
    klass = chime_class_get_class(klass);
    
    chime_class_set_instance_method(klass, name, function);
}
