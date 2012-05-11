// Chime Runtime: chime_object.c

#include "chime_object.h"
#include "runtime/support.h"
#include "chime_object_internal.h"
#include "chime_object_methods.h"
#include "runtime/chime_runtime.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/classes/array/chime_array.h"
#include "runtime/core/class/chime_class.h"
#include "runtime/core/class/chime_class_internal.h"
#include "runtime/core/class/chime_class_methods.h"
#include "runtime/core/closure/chime_closure.h"
#include "runtime/classes/float/chime_float.h"
#include "runtime/core/reference/chime_reference.h"
#include "runtime/classes/string/chime_string.h"
#include "runtime/tagging.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// #define PRINT_LIFECYCLE
// #define SKIP_RELEASES

void chime_object_initialize(void)
{
    // create the root class, Object, and it's MetaClass, which is at
    // the very top of the class heirarchy.
    _object_class = chime_class_create("Object", 0);
    
    _method_class = chime_class_create_object_subclass("Method");
    
    chime_class_set_class_method(_object_class, "new",       class_new);
    chime_class_set_class_method(_object_class, "name",      class_name);
    chime_class_set_class_method(_object_class, "==",        class_equals);
    chime_class_set_class_method(_object_class, "methods",   class_methods);
    chime_class_set_class_method(_object_class, "to_string", object_to_string);
    
    chime_class_set_instance_method(_object_class, "initialize",   object_initialize);
    chime_class_set_instance_method(_object_class, "finalize",     object_finalize);
    chime_class_set_instance_method(_object_class, "class",        object_class);
    chime_class_set_instance_method(_object_class, "superclass",   object_superclass);
    chime_class_set_instance_method(_object_class, "methods",      object_methods);
    chime_class_set_instance_method(_object_class, "retain_count", object_retain_count);
    chime_class_set_instance_method(_object_class, "invoke:",      object_invoke);
    chime_class_set_instance_method(_object_class, "hash",         object_hash);
    chime_class_set_instance_method(_object_class, "to_string",    object_to_string);
    chime_class_set_instance_method(_object_class, "nil?",         object_nil);
    chime_class_set_instance_method(_object_class, "===",          object_case_compare);
    
    chime_class_set_instance_method(_method_class, "name", method_name);
}

chime_object_t* chime_object_create_with_name(const char* class_name)
{
    chime_class_t* klass;
    
    if (chime_log_level >= 5)
        fprintf(stderr, "[runtime] creating object with class name '%s'\n", class_name);
        
    klass = chime_runtime_get_class(class_name);
    
    return chime_object_create(klass);
}

chime_object_t* chime_object_create(chime_class_t* object_class)
{
    return chime_object_raw_create(object_class, sizeof(chime_object_t), true);
}

chime_object_t* chime_object_raw_create(chime_class_t* object_class, unsigned int struct_size, unsigned char create_variables)
{
    chime_object_t* object;
    
    assert(object_class);
    
    object = (chime_object_t*)malloc(struct_size);
    
    object->self_class   = object_class;
    object->flags        = 0;
    object->retain_count = 1;
    object->methods      = chime_dictionary_create(NULL);
    
    if (create_variables)
    {
        object->variables = chime_dictionary_create((chime_collection_finalizer)chime_object_release);
        object->flags |= ObjectHasVariables;
    }
    
    return object;
}

void chime_object_destroy(chime_object_t* object)
{
    if (chime_object_is_tagged(object))
        return;
    
    // this is not quite correct, because we have to finalize all
    // super-classes
    chime_object_invoke_0(object, "finalize");
    
    if (object->self_class == _reference_class)
    {
        chime_reference_destroy(object);
        return;
    }
    
    if (object->self_class == _closure_class)
    {
        chime_closure_destroy((chime_closure_t*)object);
        return;
    }
    
    if (object->self_class == _float_class)
    {
        chime_float_destroy(object);
        return;
    }
    
    if (object->self_class == _string_class)
    {
        chime_string_destroy(object);
        return;
    }
    
    if (object->self_class == _array_class)
    {
        chime_array_destroy(object);
        return;
    }
    
    assert(object);
    
    chime_dictionary_destroy(object->methods);
    
    if (object->flags & ObjectHasVariables)
        chime_dictionary_destroy(object->variables);
    
    free(object);
}

void chime_object_retain(chime_object_t* instance)
{
    if (chime_object_is_tagged(instance))
        return;
    
#if defined(PLATFORM_MAC_OS_X)
    chime_atomic_increment32_barrier(&instance->retain_count);
#else
    instance->retain_count++;
#endif

#ifdef PRINT_LIFECYCLE
    fprintf(stderr, "[runtime] %p retain, count: %u\n", instance, instance->retain_count);
#endif
}

void chime_object_release(chime_object_t* instance)
{
    int new_count;
    
#ifdef SKIP_RELEASES
    return;
#endif
    
    if (chime_object_is_tagged(instance))
        return;
    
    if (instance->flags & ObjectIsClass)
        return;
    
#if defined(PLATFORM_MAC_OS_X)
    new_count = chime_atomic_decrement32_barrier(&instance->retain_count);
#else
    instance->retain_count--;
    new_count = instance->retain_count;
#endif
    
    if (new_count == 0)
    {
#ifdef PRINT_LIFECYCLE
        fprintf(stderr, "[runtime] %p destroy\n", instance);
#endif
        chime_object_destroy(instance);
        return;
    }
    else if (new_count < 0)
    {
        assert(0 && "Object overrelease");
    }
    
#ifdef PRINT_LIFECYCLE
    fprintf(stderr, "[runtime] %p release, count: %u\n", instance, instance->retain_count);
#endif
}

int chime_object_get_retain_count(chime_object_t* instance)
{
    return instance->retain_count;
}

chime_object_type_t chime_object_get_type(chime_object_t* instance)
{
    if (instance == CHIME_LITERAL_NULL)
        return CHIME_NULL_TYPE;
    
    if (!chime_object_is_tagged(instance))
        return CHIME_OBJECT_TYPE;
    
    if (chime_object_is_integer(instance))
        return CHIME_INTEGER_TYPE;
    
    if (chime_object_is_boolean(instance))
        return CHIME_BOOLEAN_TYPE;
    
    return CHIME_OBJECT_INVALID_TYPE;
}

chime_class_t* chime_object_get_class(chime_object_t* instance)
{
    switch (chime_object_get_type(instance))
    {
        case CHIME_OBJECT_TYPE:
            break;
        case CHIME_NULL_TYPE:
            return chime_runtime_get_class("Null");
            break;
        case CHIME_INTEGER_TYPE:
            return chime_runtime_get_class("Integer");
            break;
        case CHIME_BOOLEAN_TYPE:
            return chime_runtime_get_class("Boolean");
            break;
        default:
            assert(0 && "This type of object isn't handled yet");
            break;
    }
    
    return instance->self_class;
}

chime_object_t* chime_object_get_superclass(chime_object_t* instance)
{
    chime_class_t* klass;
    
    klass = (chime_class_t*)chime_object_get_class(instance);
    
    return (chime_object_t*)chime_class_get_superclass(klass);
}

chime_dictionary_t* chime_object_get_methods(chime_object_t* instance)
{
    assert(instance);
    
    return instance->methods;
}

chime_dictionary_t* chime_object_get_attributes(chime_object_t* instance)
{
    assert(instance);
    
    return instance->variables;
}

void chime_object_print(struct _chime_object* instance)
{
    chime_dictionary_t*    dict;
    chime_runtime_array_t* keys;
    unsigned long          i;

    dict = chime_object_get_attributes(instance);
    keys = chime_dictionary_get_keys(dict);
    
    for (i = 0; i < chime_runtime_array_count(keys); ++i)
    {
        char* key;
        
        key = chime_runtime_array_get(keys, i);
        fprintf(stderr, "[runtime] %s => %p\n", key, chime_dictionary_get(dict, key));
    }
}

chime_object_t* chime_object_get_attribute(chime_object_t* instance, const char* name)
{
    chime_object_t* object;
    
    object = chime_object_get_attribute_unretained(instance, name);
    chime_object_retain(object);
    
    // The retain here does seem a little funny.  The problem is we need to be sure
    // that we satisfy the +1 rule.  Getting an atttribute acts like a function
    // call.
    
    return object;
}

chime_object_t* chime_object_get_attribute_unretained(chime_object_t* instance, const char* name)
{
    if (!instance)
        return CHIME_LITERAL_NULL;
    
    if (!instance->variables)
        return CHIME_LITERAL_NULL;
    
    return (chime_object_t*)chime_dictionary_get(instance->variables, name);
}

void chime_object_set_property(chime_object_t* instance, const char* name, chime_object_t* value)
{
    if (!instance->variables)
    {
        instance->variables = chime_dictionary_create((chime_collection_finalizer)chime_object_release);
    }
    
    chime_object_retain(value);
    chime_dictionary_set(instance->variables, name, value);
}

void chime_object_set_attribute(chime_object_t* instance, const char* name, chime_object_t* value)
{
    chime_object_set_property(instance, name, value);
}

void* chime_object_resolve_invoke(chime_object_t* instance, const char* name)
{
    chime_class_t*         klass;
    void*                  function;
    chime_runtime_array_t* trait_array;
    unsigned long          i;
    
    function = NULL;
    
    klass = chime_object_get_class(instance);
    assert(klass);
    
    // we now need to hunt up the inheritance chain for the right method
    do
    {
        // check the class functions
        function = chime_dictionary_get(chime_object_get_methods((chime_object_t*)klass), name);
        if (function)
            break;
        
        // no dice, so check our traits
        trait_array = klass->traits;
        if (trait_array)
        {
            for (i = 0; i < chime_runtime_array_count(trait_array); ++i)
            {
                chime_class_t* trait;
                
                trait = chime_runtime_array_get(trait_array, i);
                function = chime_dictionary_get(chime_object_get_methods((chime_object_t*)trait), name);
                if (function)
                    break;
            }
            
            if (function)
                break;
        }
        
        // Be careful here.  We need to use the chime_class_x methods, because
        // we know we have a class instance.  This is one of the few places where the
        // interchangable-ness of classes and objects breaks down
        klass = chime_class_get_superclass(klass);
    } while (klass);
    
    if (!function)
    {
        chime_object_t* string;
        
        string = object_to_string(instance);
        
        if (chime_log_level >= 4)
            fprintf(stderr, "[runtime] method missing for '%s' on %s\n", name, chime_string_to_c_string(string));
    }
    
    return function;
}

chime_object_t* chime_object_invoke_0(chime_object_t* instance, const char* name)
{
    void* function;
    
    function = chime_object_resolve_invoke(instance, name);
    
    if (!function)
        return CHIME_LITERAL_NULL;
    
    return ((chime_function_arg0_t)function)(instance);
}

chime_object_t* chime_object_invoke_1(chime_object_t* instance, const char* name, chime_object_t* arg1)
{
    void* function;
    
    function = chime_object_resolve_invoke(instance, name);
    
    if (!function)
        return CHIME_LITERAL_NULL;
    
    return ((chime_function_arg1_t)function)(instance, arg1);
}

chime_object_t* chime_object_invoke_2(chime_object_t* instance, const char* name, chime_object_t* arg1, chime_object_t* arg2)
{
    void* function;
    
    function = chime_object_resolve_invoke(instance, name);
    
    if (!function)
        return CHIME_LITERAL_NULL;
    
    return ((chime_function_arg2_t)function)(instance, arg1, arg2);
}

chime_object_t* chime_object_invoke_3(chime_object_t* instance, const char* name, chime_object_t* arg1, chime_object_t* arg2, chime_object_t* arg3)
{
    void* function;
    
    function = chime_object_resolve_invoke(instance, name);
    
    if (!function)
        return CHIME_LITERAL_NULL;
    
    return ((chime_function_arg3_t)function)(instance, arg1, arg2, arg3);
}
