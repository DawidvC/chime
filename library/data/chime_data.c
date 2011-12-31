#include "chime_data.h"
#include "runtime/classes/array/chime_array.h"
#include "runtime/classes/array/chime_array_methods.h"
#include "runtime/classes/boolean/chime_boolean.h"
#include "runtime/classes/integer/chime_integer.h"
#include "runtime/classes/string/chime_string.h"
#include "runtime/classes/string/chime_string_methods.h"
#include "runtime/core/closure/chime_closure.h"
#include "runtime/chime_runtime_internal.h"
#include "library/extensions/chime_string_extensions.h"
#include "runtime/platform.h"
#include "runtime/tagging.h"

#include <stdio.h>

void chime_data_initialize(void)
{
    chime_class_t* klass;
    
    klass = chime_class_create_object_subclass("Data");
    
    // instance methods
    chime_class_set_instance_method(klass, "initialize",   data_initialize);
    chime_class_set_instance_method(klass, "finalize",     data_finalize);
    
    chime_class_set_instance_method(klass, "length",       data_get_length);
    chime_class_set_instance_method(klass, "append:size:", data_append_block);
    chime_class_set_instance_method(klass, "to_string",    data_to_string);
    
    // extend string
    chime_class_set_instance_method(_string_class, "to_data",   string_to_data);
}

chime_object_t* data_initialize(chime_object_t* instance)
{
    chime_object_t* array;
    
    array = chime_array_create();
    chime_object_set_attribute(instance, "_data_array", array);
    chime_object_release(array);
    
    array = chime_array_create();
    chime_object_set_attribute(instance, "_size_array", array);
    chime_object_release(array);
    
    chime_object_set_attribute(instance, "_should_free", CHIME_FALSE);
    
    chime_object_set_attribute(instance, "_length", chime_integer_encode(0));
    
    return instance;
}

chime_object_t* data_finalize(chime_object_t* instance)
{
    // signed long     i;
    // signed long     length;
    // chime_object_t* array;
    // bool            should_free;
    // 
    // // we need to go through and delete everything from the instance
    // 
    // array       = chime_object_get_attribute(instance, "_array");
    // should_free = chime_boolean_decode(chime_object_get_attribute(instance, "_should_free"));
    // 
    // if (!should_free)
    //     return CHIME_NIL;
    // 
    // length = array_length(array);
    // 
    // for (i = 0; i < length; ++i)
    // {
    //     void* data_block;
    //     
    //     data_block = array_indexer_get(array, chime_integer_encode(i));
    //     
    //     free(chime_tag_decode_raw_block(data_block));
    // }
    
    return CHIME_NIL;
}

chime_object_t* data_get_length(chime_object_t* instance)
{
    return chime_object_get_attribute(instance, "_length");
}

chime_object_t* data_append_block(chime_object_t* instance, chime_object_t* block, chime_object_t* size)
{
    signed long     length;
    chime_object_t* array;
    
    array = chime_object_get_attribute_unretained(instance, "_data_array");
    array_append(array, block);
    
    array = chime_object_get_attribute_unretained(instance, "_size_array");
    array_append(array, size);
    
    length  = chime_integer_decode(chime_object_get_attribute_unretained(instance, "_length"));
    length += chime_integer_decode(size);
    
    chime_object_set_attribute(instance, "_length", chime_integer_encode(length));
    
    return CHIME_NIL;
}

chime_object_t* data_append_raw_block(chime_object_t* instance, void* ptr, unsigned long size)
{
    return data_append_block(instance, chime_tag_encode_raw_block(ptr), chime_integer_encode(size));
}

chime_object_t* data_each(chime_object_t* instance, chime_object_t* function)
{
    chime_object_t* data_array;
    chime_object_t* size_array;
    unsigned long   length;
    signed long     i;
    
    data_array = chime_object_get_attribute_unretained(instance, "_data_array");
    size_array = chime_object_get_attribute_unretained(instance, "_size_array");
    
    length = chime_integer_decode(chime_object_invoke_0(data_array, "length"));
    for (i = 0; i < length; ++i)
    {
        chime_object_t* data;
        chime_object_t* size;
        
        data = chime_object_invoke_1(data_array, "[]:", chime_integer_encode(i));
        size = chime_object_invoke_1(size_array, "[]:", chime_integer_encode(i));
        
        chime_closure_invoke_2((chime_closure_t*)function, data, size);
    }
    
    return CHIME_NIL;
}

chime_object_t* data_to_string(chime_object_t* instance)
{
    chime_object_t* string;
    chime_object_t* data_array;
    chime_object_t* size_array;
    unsigned long   length;
    signed long     i;
    
    string     = chime_string_create();
    data_array = chime_object_get_attribute_unretained(instance, "_data_array");
    size_array = chime_object_get_attribute_unretained(instance, "_size_array");
    
    length = chime_integer_decode(chime_object_invoke_0(data_array, "length"));
    for (i = 0; i < length; ++i)
    {
        chime_object_t* block_string;
        chime_object_t* data;
        chime_object_t* size;
        
        data = chime_object_invoke_1(data_array, "[]:", chime_integer_encode(i));
        size = chime_object_invoke_1(size_array, "[]:", chime_integer_encode(i));
        
        block_string = chime_string_create_with_c_string(chime_tag_decode_raw_block(data), chime_integer_decode(size));
        
        string_add(string, block_string);
        
        chime_object_release(block_string);
    }
    
    return string;
}

chime_object_t* chime_data_get_block(chime_object_t* instance, chime_object_t* index)
{
    chime_object_t* data_array;
    
    data_array = chime_object_get_attribute_unretained(instance, "_data_array");
    
    return chime_object_invoke_1(data_array, "[]:", index);
}

chime_object_t* chime_data_get_block_size(chime_object_t* instance, chime_object_t* index)
{
    chime_object_t* size_array;
    
    size_array = chime_object_get_attribute_unretained(instance, "_size_array");
    
    return chime_object_invoke_1(size_array, "[]:", index);
}
