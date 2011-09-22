// Chime Runtime: chime_string.c

#include "chime_string.h"
#include "chime_string_internal.h"
#include "chime_string_methods.h"
#include "runtime/chime_runtime.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/class/chime_class.h"
#include "runtime/object/chime_object_internal.h"

#include <stdio.h>
#include <assert.h>

void chime_string_initialize(void)
{
    _string_class = chime_class_create_object_subclass("String");
    
    // class methods
    chime_class_set_class_method(_string_class, "new", string_class_new);
    
    chime_class_set_instance_method(_string_class, "to_string", string_to_string);
    chime_class_set_instance_method(_string_class, "print",     string_print);
    chime_class_set_instance_method(_string_class, "<=>",       string_compare);
    chime_class_set_instance_method(_string_class, "[]",        string_indexer_get);
    chime_class_set_instance_method(_string_class, "length",    string_length_get);
}

void chime_string_destroy(void)
{
    chime_class_destroy(_string_class);
    _string_class = NULL;
}

chime_object_t* chime_string_create_with_c_string(const char* string, unsigned int length)
{
    chime_object_t*        instance;
    chime_runtime_array_t* array;
    unsigned int           i;
    unsigned int           count;
    char*                  data_chunk;
    
    assert(string);
    
    instance = chime_runtime_instantiate(_string_class);
    assert(instance);
    
    array = string_get_internal_array(instance);
    count = BYTES_PER_STRING_STORAGE_UNIT; // make sure an allocation happens right away
    
    // loop through the string, copying one byte at a time into the current chunk.  If we
    // fill up a chunk, make a new one, add it to the internal array and continue
    for (i = 0; i < length; ++i, ++count)
    {
        if (count >= BYTES_PER_STRING_STORAGE_UNIT)
        {
            data_chunk = chime_allocate(STRING_STORAGE_UNIT);
            chime_runtime_array_add(array, data_chunk);
            count      = 0;
        }
        
        data_chunk[count] = string[i];
    }
    
    // make sure that anything left over in the data_chunk is zero'ed out
    for (; count < BYTES_PER_STRING_STORAGE_UNIT; ++count)
    {
        data_chunk[count] = 0;
    }
    
    chime_object_set_attribute(instance, "_length", chime_literal_encode_integer(length));
    
    return instance;
}

char* chime_string_to_c_string(chime_object_t* instance)
{
    char*                  buffer;
    chime_runtime_array_t* array;
    unsigned int           i;
    unsigned int           j;
    unsigned int           length;
    unsigned int           cursor;
    
    assert(instance && "Trying to convert NULL to a c-string");
    
    buffer = (char*)chime_object_get_attribute(instance, "_c_string");
    
    if (buffer)
        return buffer;
    
    // we need to build a contiguous buffer to fit the string, so first
    // we need to know how many bytes long it is.  That is <= the length
    // of the internal array times the storage of each of those elements
    array  = string_get_internal_array(instance);
    length = chime_runtime_array_count(array);
    
    buffer = chime_allocate(length * STRING_STORAGE_UNIT);
    cursor = 0;
    
    for (i = 0; i < length; ++i)
    {
        char* data_chunk;
        
        data_chunk = chime_runtime_array_get(array, i);
        for (j = 0; j < BYTES_PER_STRING_STORAGE_UNIT; ++j)
        {
            buffer[cursor] = data_chunk[j];
            
            cursor += 1;
        }
    }
    
    chime_object_set_attribute(instance, "_c_string", (chime_object_t*)buffer);
    
    return buffer;
}
