// Chime Runtime: chime_runtime.c

#include "runtime/chime_runtime.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/class/chime_class.h"
#include "runtime/collections/chime_runtime_array.h"
#include "runtime/object/chime_object_internal.h"
#include "runtime/string/chime_string.h"
#include "runtime/array/chime_array.h"
#include "runtime/hash/chime_hash.h"
#include "runtime/reference/chime_reference.h"
#include "runtime/closure/chime_closure.h"
#include "runtime/range/chime_range.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

chime_dictionary_t* _chime_classes   = NULL;
chime_class_t*      _object_class    = NULL;
chime_class_t*      _string_class    = NULL;
chime_class_t*      _array_class     = NULL;
chime_class_t*      _hash_class      = NULL;
chime_class_t*      _range_class     = NULL;
chime_class_t*      _method_class    = NULL;
chime_class_t*      _undefined_class = NULL;
unsigned char       chime_log_level  = 4;

void chime_runtime_initialize(void)
{
    unsigned char old_level;
    
    // save original logging level
    old_level = chime_log_level;
    chime_log_level = 3;
    
    _chime_classes = chime_dictionary_create();
    
    chime_object_initialize(); // set up the root classes and their base methods
    
    chime_literal_initialize();
    
    chime_string_initialize();
    
    chime_array_initialize();
    
    chime_hash_initialize();
    
    chime_reference_initialize();
    
    chime_closure_initialize();
    
    chime_range_initialize();
    
    chime_log_level = old_level;
}

void chime_runtime_destroy(void)
{
    // this should also remove all of the contained objects
    chime_dictionary_destroy(_chime_classes);
    _chime_classes = NULL;
    
    _object_class    = NULL;
    _string_class    = NULL;
    _array_class     = NULL;
    _hash_class      = NULL;
    _range_class     = NULL;
    _method_class    = NULL;
    _undefined_class = NULL;
}

chime_object_t* chime_runtime_create_class(const char* name, chime_object_t* superclass)
{
    return (chime_object_t*)chime_class_create(name, (chime_class_t*)superclass);
}

void chime_runtime_set_instance_method(chime_object_t* klass, const char* name, void* function)
{
    chime_class_set_instance_method((chime_class_t*)klass, name, function);
}

void chime_runtime_set_class_method(chime_object_t* klass, const char* name, void* function)
{
    chime_class_set_class_method((chime_class_t*)klass, name, function);
}

chime_class_t* chime_runtime_get_class(const char* name)
{
    chime_class_t* klass;
    
    klass = chime_dictionary_get(_chime_classes, name);
    
    if (chime_log_level >= 5)
        fprintf(stderr, "[runtime] Getting class '%s' => '%s'\n", name, chime_runtime_get_class_name(klass));
    
    if (chime_log_level >= 4 && klass == NULL)
    {
        fprintf(stderr, "[runtime] Getting class '%s' returned null\n", name);
        klass = _undefined_class;
    }
    
    return klass;
}

char* chime_runtime_get_class_name(chime_class_t* klass)
{
    char* name;
    
    assert(klass);
    
    name = chime_dictionary_get_key(_chime_classes, klass);
    if (!name)
        return "UndefinedClassName";
    
    return name;
}

chime_object_t* chime_runtime_instantiate(chime_class_t* klass)
{
    assert(klass);
    
    return chime_object_invoke_0((chime_object_t*)klass, "new");
}

chime_object_t* chime_runtime_load(const char* name)
{
    fprintf(stderr, "[runtime] loading '%s'\n", name);
    
    return CHIME_LITERAL_TRUE;
}
