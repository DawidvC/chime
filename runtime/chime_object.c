// Chime Runtime: chime_object.c

#include "chime_object.h"
#include "chime_object_internal.h"
#include "chime_runtime.h"
#include "chime_runtime_internal.h"
#include "chime_literals.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

static chime_object_t* ObjectClass(chime_object_t* instance, const char* method_name, ...);
static chime_object_t* ObjectToString(chime_object_t* instance, const char* method_name, ...);
static chime_object_t* object_methods(chime_object_t* instance, const char* method_name, ...);

void chime_object_initialize(void)
{
    chime_object_set_function(_object_class, "class",     ObjectClass,    0);
    chime_object_set_function(_object_class, "to_string", ObjectToString, 0);
    chime_object_set_function(_object_class, "methods",   object_methods, 0);
}

chime_object_t* chime_object_create(chime_object_t* object_class)
{
    chime_object_t* object;
    
    object = (chime_object_t*)malloc(sizeof(chime_object_t));
    
    object->flags           = 0;
    object->reference_count = 1;
    
    object->self_class  = object_class;
    
    if (object_class)
    {
        object->super_class = object_class->super_class;
    }
    else
    {
        object->super_class = 0;
    }    
    
    object->properties = 0;
    
    return object;
}

chime_object_t* chime_object_create_with_name(const char* class_name)
{
    chime_object_t* object_class;
    
    if (chime_log_level >= 5)
        fprintf(stderr, "[runtime] creating object with class name '%s'\n", class_name);
        
    object_class = chime_runtime_get_class(class_name);
    
    // class must not be null
    assert(object_class);
    
    return chime_object_create(object_class);
}

void chime_object_destroy(chime_object_t* object)
{
    if (chime_object_is_literal(object))
        return;
    
    if (object->properties)
    {
        free(object->properties);
    }
    
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
    switch (chime_object_get_type(instance))
    {
        case CHIME_OBJECT_TYPE:
            break;
        case CHIME_NULL_TYPE:
        case CHIME_INTEGER_TYPE:
        case CHIME_BOOLEAN_TYPE:
            return chime_runtime_get_class("Object");
            break;
        default:
            assert(0 && "This type of object isn't handled yet");
            break;
    }
    
    return instance->super_class;
}

chime_object_t* chime_object_get_property(chime_object_t* instance, const char* name)
{
    if (!instance)
        return CHIME_LITERAL_NULL;
    
    if (!instance->properties)
        return CHIME_LITERAL_NULL;
    
    return (chime_object_t*)chime_dictionary_get(instance->properties, name);
}

void chime_object_set_property(chime_object_t* instance, const char* name, chime_object_t* value)
{
    if (!instance->properties)
    {
        instance->properties = chime_dictionary_create();
    }
    
    chime_dictionary_set(instance->properties, name, value);
}

chime_function_t chime_object_get_function(chime_object_t* instance)
{
    return (chime_function_t)instance->properties;
}

void chime_object_set_function(chime_object_t* instance, const char* name, chime_function_t function, unsigned long arity)
{
    chime_object_t* method_object;
    
    if (chime_log_level >= 5)
        fprintf(stderr, "[runtime] setting function '%s' on %p\n", name, instance);
        
    assert(instance);
    assert(name);
    assert(arity <= 8);
    
    method_object = chime_object_create_with_name("Method");
    method_object->properties = (void*)function;
    method_object->flags = arity;
    
    chime_object_set_property(instance, name, method_object);
}

chime_object_t* chime_object_resolve_invoke(chime_object_t* instance, const char* name)
{
    chime_object_t*  class_object;
    chime_object_t*  method_object;
    
    method_object = NULL;
    
    class_object = chime_object_get_class(instance);
    
    assert(class_object);
    
    // we now need to hunt up the inheritance chain for the right method
    do
    {
        method_object = chime_object_get_property(class_object, name);
        if (method_object)
            break;
        
        class_object = chime_object_get_superclass(class_object);
    } while (class_object);
    
    return method_object;
}

chime_object_t* chime_object_invoke(chime_object_t* instance, const char* name, ...)
{
    chime_function_t function;
    chime_object_t*  method_object;
    chime_object_t*  result_object;
    va_list          arguments;
    
    method_object = chime_object_resolve_invoke(instance, name);
    if (!method_object)
    {
        chime_object_t*  class_object;
        chime_object_t*  class_name;
        
        class_object = chime_object_get_class(instance);
        
        class_name = ObjectToString(class_object, "to_string");
        
        if (chime_log_level >= 4)
            fprintf(stderr, "[runtime] method missing for '%s' on %s\n", name, chime_string_to_c_string(class_name));
            
        return 0;
    }
    
    function = chime_object_get_function(method_object);
    
    va_start(arguments, name);
    
    switch (method_object->flags)
    {
        case 0:
            result_object = function(instance, name);
            break;
        case 1:
            result_object = function(instance, name, va_arg(arguments, chime_object_t*));
            break;
        case 2:
            result_object = function(instance, name, va_arg(arguments, chime_object_t*), va_arg(arguments, chime_object_t*));
            break;
    }
    
    va_end(arguments);
    
    return result_object;
}

#pragma mark -
#pragma mark Method Definitions of Object
static chime_object_t* ObjectClass(chime_object_t* instance, const char* method_name, ...)
{
    return chime_object_get_class(instance);
}

static chime_object_t* ObjectToString(chime_object_t* instance, const char* method_name, ...)
{
    int             string_length;
    char*           buffer;
    chime_object_t* string;
    
    // we have to add the size of the "<:0x12345678>" to the string
    string_length = strlen(method_name) + 13;
    
    buffer = malloc(string_length + 1);
    
    string = chime_object_get_property(instance, "name");
    
    snprintf(buffer, string_length, "<%s:%p>", chime_string_to_c_string(string), instance);
    
    string = chime_string_create_with_c_string(buffer);
    
    // here, we should free the buffer
    // except right now the string class doesn't make a copy
    // free(buffer);
    
    return string;
}

static chime_object_t* object_methods(chime_object_t* instance, const char* method_name, ...)
{
    return CHIME_LITERAL_NULL;
}