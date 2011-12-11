// Chime Runtime: chime_class.c

#include "chime_class.h"
#include "chime_class_internal.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/chime_runtime.h"
#include "runtime/core/object/chime_object.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

chime_class_t*  chime_class_create_object_subclass(const char* name)
{
    return chime_class_create(name, _object_class);
}

chime_class_t* chime_class_create(const char* name, chime_class_t* superclass)
{
    chime_class_t*  klass;
    chime_class_t*  metaclass;
    char*           metaclass_name;
    
    metaclass_name = malloc(strlen(name) + 4);
    metaclass_name[0] = 'M';
    metaclass_name[1] = 'e';
    metaclass_name[2] = 't';
    metaclass_name[3] = 'a';
    
    strcpy(metaclass_name+4, name);
    
    // create the metaclass first
    metaclass = (chime_class_t*)malloc(sizeof(chime_class_t));
    
    // entity portion
    metaclass->object.methods      = chime_dictionary_create(NULL);
    metaclass->object.self_class   = NULL;
    metaclass->object.flags        = ObjectIsClass;
    metaclass->object.retain_count = 0;
    
    // class portion
    metaclass->traits = NULL;
    
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
    klass->object.methods      = chime_dictionary_create(NULL);
    klass->object.self_class   = metaclass;
    klass->object.flags        = ObjectIsClass;
    klass->object.retain_count = 0;
    klass->object.variables    = chime_dictionary_create((chime_collection_finalizer)chime_object_release);
    
    // class portion
    klass->super_class = superclass;
    klass->traits      = NULL;
    
    assert(klass);
    
    if ((superclass == _module_class) && superclass && _module_class)
    {
        chime_dictionary_set(_chime_modules, name, klass);
        chime_dictionary_set(_chime_modules, metaclass_name, metaclass);
        //fprintf(stderr, "Creating module %s => %p, %p, %p\n", name, klass, metaclass, superclass);
    }
    else
    {
        chime_dictionary_set(_chime_classes, name, klass);
        chime_dictionary_set(_chime_classes, metaclass_name, metaclass);
        //fprintf(stderr, "Creating class %s => %p, %p, %p\n", name, klass, metaclass, superclass);
    }
    
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

chime_object_t* chime_class_instantiate(chime_class_t* klass)
{
    assert(klass);
    
    return chime_object_invoke_0((chime_object_t*)klass, "new");
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

void chime_class_include_trait(chime_class_t* klass, chime_class_t* trait)
{
    assert(klass);
    assert(trait);
    
    if (!klass->traits)
        klass->traits = chime_runtime_array_create(NULL);
    
    chime_runtime_array_add(klass->traits, trait);
}
