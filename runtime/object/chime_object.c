// Chime Runtime: chime_object.c

#include "chime_object.h"
#include "chime_object_internal.h"
#include "chime_object_methods.h"
#include "runtime/chime_runtime.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/class/chime_class_methods.h"
#include "runtime/string/chime_string.h"
#include "runtime/literals/chime_literal.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void chime_object_initialize(void)
{
    // create the root class, Object, and it's MetaClass, which is at
    // the very top of the class heirarchy.
    _object_class = chime_class_create("Object", 0);
    
    _method_class = chime_class_create_object_subclass("Method");
    
    chime_class_set_class_method(_object_class,  "new",     class_new); 
    chime_class_set_class_method(_object_class,  "name",    class_name); 
    chime_class_set_class_method(_object_class,  "<=>",     class_compare);
    chime_class_set_class_method(_object_class,  "methods", class_methods);
    
    chime_class_set_instance_method(_object_class, "class",     object_class);
    chime_class_set_instance_method(_object_class, "to_string", object_to_string);
    chime_class_set_instance_method(_object_class, "methods",   object_methods);
    chime_class_set_instance_method(_object_class, "==",        object_equals);
    chime_class_set_instance_method(_object_class, ">",         object_greater_than);
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
    chime_object_t* object;
    
    assert(object_class);
    
    object = (chime_object_t*)malloc(sizeof(chime_object_t));
    
    object->self_class = object_class;
    object->flags      = 0;
    object->methods    = chime_dictionary_create();
    object->variables  = chime_dictionary_create();
    
    return object;
}

void chime_object_destroy(chime_object_t* object)
{
    if (chime_object_is_literal(object))
        return;
    
    assert(object);
    
    free(object->methods);
    free(object->variables);
    
    free(object);
}

chime_object_type_t chime_object_get_type(chime_object_t* instance)
{
    if (instance == CHIME_LITERAL_NULL)
        return CHIME_NULL_TYPE;
    
    if (!chime_object_is_literal(instance))
        return CHIME_OBJECT_TYPE;
    
    if (chime_object_is_integer(instance))
        return CHIME_INTEGER_TYPE;
    
    if (chime_object_is_boolean(instance))
        return CHIME_BOOLEAN_TYPE;
    
    return CHIME_OBJECT_INVALID_TYPE;
}

chime_object_t* chime_object_get_class(chime_object_t* instance)
{
    switch (chime_object_get_type(instance))
    {
        case CHIME_OBJECT_TYPE:
            break;
        case CHIME_NULL_TYPE:
            return (chime_object_t*)chime_runtime_get_class("Null");
            break;
        case CHIME_INTEGER_TYPE:
            return (chime_object_t*)chime_runtime_get_class("Integer");
            break;
        case CHIME_BOOLEAN_TYPE:
            return (chime_object_t*)chime_runtime_get_class("Boolean");
            break;
        default:
            assert(0 && "This type of object isn't handled yet");
            break;
    }
    
    return (chime_object_t*)instance->self_class;
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

chime_object_t* chime_object_get_property(chime_object_t* instance, const char* name)
{
    if (!instance)
        return CHIME_LITERAL_NULL;
    
    if (!instance->variables)
        return CHIME_LITERAL_NULL;
    
    return (chime_object_t*)chime_dictionary_get(instance->variables, name);
}

chime_object_t* chime_object_get_attribute(chime_object_t* instance, const char* name)
{
    return chime_object_get_property(instance, name);
}


void chime_object_set_property(chime_object_t* instance, const char* name, chime_object_t* value)
{
    if (!instance->variables)
    {
        instance->variables = chime_dictionary_create();
    }
    
    chime_dictionary_set(instance->variables, name, value);
}

void chime_object_set_attribute(chime_object_t* instance, const char* name, chime_object_t* value)
{
    chime_object_set_property(instance, name, value);
}

void* chime_object_resolve_invoke(chime_object_t* instance, const char* name)
{
    chime_object_t* klass;
    void*           function;
    
    function = NULL;
    
    klass = chime_object_get_class(instance);
    assert(klass);
    
    // we now need to hunt up the inheritance chain for the right method
    do
    {
        function = chime_dictionary_get(chime_object_get_methods(klass), name);
        if (function)
            break;
        
        // Be careful here.  We need to use the chime_class_x methods, because
        // we know we have a class instance.  This is one of the few places where the
        // interchangable-ness of classes and objects breaks down
        klass = (chime_object_t*)chime_class_get_superclass((chime_class_t*)klass);
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
