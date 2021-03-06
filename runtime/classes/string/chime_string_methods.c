// Chime Runtime: chime_string_methods.c

#include "chime_string_methods.h"
#include "chime_string_internal.h"
#include "chime_string.h"
#include "runtime/core/object/chime_object.h"
#include "runtime/classes/integer/chime_integer.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/support.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

chime_object_t* string_class_new(chime_class_t* klass)
{
    chime_object_t*        instance;
    chime_runtime_array_t* internal_array;
    
    // first thing to do is invoke new on super
    instance       = chime_object_create(klass);
    internal_array = chime_runtime_array_create(NULL);
    
    // because we are storing a non-object type, we have to do this by reaching into the
    // object internals.  This is definitely less than ideal.
    chime_dictionary_set(instance->variables, "_internal_array", (chime_object_t*)internal_array);
    
    string_length_set(instance, 0);
    string_set_buffer(instance, STRING_EMPTY_BUFFER_FLAG);
    
    return instance;
}

chime_object_t* string_to_string(chime_object_t* instance)
{
    chime_object_retain(instance);
    
    return instance;
}

chime_object_t* string_print(chime_object_t* instance)
{
    fprintf(stdout, "%s\n", chime_string_to_c_string(instance));
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* string_compare(chime_object_t* instance, chime_object_t* other)
{
    chime_object_t* other_as_string;
    const char*     s1;
    const char*     s2;
    int             result;
    
    other_as_string = chime_object_invoke_0(other, "to_string");
    if (other_as_string == CHIME_LITERAL_NULL)
        return CHIME_LITERAL_NULL; // incomparable
    
    s1 = chime_string_to_c_string(instance);
    s2 = chime_string_to_c_string(other_as_string);
    
    chime_object_release(other_as_string);
    
    result = strcmp(s1, s2);
    
    if (result > 0)
        return chime_integer_encode(1);
    
    if (result < 0)
        return chime_integer_encode(-1);
    
    return chime_integer_encode(0);
}

chime_object_t* string_add(chime_object_t* instance, chime_object_t* other)
{
    chime_object_t*        other_as_string;
    signed long            instance_length;
    signed long            other_length; 
    unsigned long          instance_index;
    unsigned long          other_index;
    unsigned long          i;
    chime_runtime_array_t* instance_array;
    chime_runtime_array_t* other_array;
    unsigned long          instance_offset;
    unsigned long          other_offset;
    char*                  instance_data_chunk;
    char*                  other_data_chunk;
    
    other_as_string = chime_object_invoke_0(other, "to_string");
    
    instance_length = chime_integer_decode(string_length_get(instance));
    other_length    = chime_integer_decode(string_length_get(other_as_string));
    
    // This function does not currently satisfy the +1 garantee, and messes everything up.
    // Retaining doesn't seem to fix it.
    chime_object_retain(instance);
    
    // special-case when the other string has zero length
    if (other_length <= 0)
    {
        assert(other_length == 0);
        chime_object_release(other_as_string);
        return instance;
    }
    
    instance_array  = string_get_internal_array(instance);
    other_array     = string_get_internal_array(other_as_string);
    
    // special case when this instance has zero length
    if (instance_length <= 0)
    {
        assert(instance_length == 0);
        
        chime_runtime_array_add(instance_array, chime_allocate(STRING_STORAGE_UNIT));
    }
    
    instance_index  = instance_length / BYTES_PER_STRING_STORAGE_UNIT; // compute the array indices
    other_index     = other_length / BYTES_PER_STRING_STORAGE_UNIT;
    
    instance_offset = instance_length % BYTES_PER_STRING_STORAGE_UNIT; // compute how far into the last chunk we are
    other_offset    = 0;
    
    instance_data_chunk = chime_runtime_array_get(instance_array, instance_index);
    other_data_chunk    = chime_runtime_array_get(other_array,    other_index);
    
    for (i = 0; i < other_length; ++i, ++other_offset, ++instance_offset)
    {
        if (other_offset >= BYTES_PER_STRING_STORAGE_UNIT)
        {
            other_index += 1;
            other_offset = 0;
            other_data_chunk = chime_runtime_array_get(other_array, other_index);
        }
        
        // if we fall off the end of the chunk, we need to start adding chunks
        if (instance_offset >= BYTES_PER_STRING_STORAGE_UNIT)
        {
            instance_offset = 0;
            instance_data_chunk = chime_allocate(STRING_STORAGE_UNIT);
            chime_runtime_array_add(instance_array, instance_data_chunk);
        }
        
        instance_data_chunk[instance_offset] = other_data_chunk[other_offset];
    }
    
    // make sure that anything left over in the data_chunk is zero'ed out
    for (; instance_offset < BYTES_PER_STRING_STORAGE_UNIT; ++instance_offset)
    {
        instance_data_chunk[instance_offset] = 0;
    }
    
    // adjust the length and clear the internal buffer
    string_length_set(instance, instance_length + other_length);
    string_set_buffer(instance, STRING_EMPTY_BUFFER_FLAG);
    
    assert((instance_length + other_length) == chime_integer_decode(string_length_get(instance)));
    
    chime_object_release(other_as_string);
    
    return instance; // to allow chaining, this is important
}

chime_object_t* string_indexer_get(chime_object_t* instance, chime_object_t* index)
{
    chime_class_t* index_class;
    
    assert(instance);
    assert(index);
    
    index_class = chime_object_get_class(index);
    if (index_class == _range_class)
    {
        signed long start;
        signed long end;
        char*       c_string;
        signed long length;
        
        start  = chime_integer_decode(chime_object_invoke_0(index,    "start"));
        end    = chime_integer_decode(chime_object_invoke_0(index,    "end"));
        length = chime_integer_decode(chime_object_invoke_0(instance, "length"));
        
        c_string = chime_string_to_c_string(instance);
        
        // fprintf(stderr, "a start %ld, end %ld, length %ld\n", start, end, length);
        
        if (end < 0)
            end += length;
        
        // fprintf(stderr, "b start %ld, end %ld, length %ld\n", start, end, length);
        assert((end <= length) && "Range.end must be <= string length");
        
        if (start < 0)
            start += length;
        
        // fprintf(stderr, "c start %ld, end %ld, length %ld\n", start, end, length);
        assert((start < length) && "Range.start must be < string length");
        
        length = end - start;
        c_string += start;
        
        assert(length >= 0 && "New length must be >= 0");
        
        // fprintf(stderr, "d start %ld, end %ld, length %ld\n", start, end, length);
        
        return chime_string_create_with_c_string(c_string, length + 1);
    }
    else
    {
        fprintf(stderr, "Class %p\n", index_class);
        
        chime_runtime_array_t* array;
        
        array = chime_dictionary_get_keys(_chime_classes);
        unsigned int i;
        for (i = 0; i < chime_runtime_array_count(array); ++i)
        {
            void* key;
            
            key = chime_runtime_array_get(array, i);
            fprintf(stderr, "%s => %p\n", key, chime_dictionary_get(_chime_classes, key));
        }
        
        assert(0 && "Indexing into a string with anything other than a Range is unimplemented");
    }
    
    return instance;
}

chime_object_t* string_length_get(chime_object_t* instance)
{
    return chime_integer_encode(((chime_string_t*)instance)->length);
}

chime_object_t* string_length_set(chime_object_t* instance, signed long length)
{
    ((chime_string_t*)instance)->length = length;
    
    return CHIME_LITERAL_NULL;
}

chime_object_t* string_concatenate(chime_object_t* instance, chime_object_t* other)
{
    chime_object_retain(instance);
    
    return instance;
}

chime_runtime_array_t* string_get_internal_array(chime_object_t* instance)
{
    return ((chime_string_t*)instance)->internal_array;
}

char* string_get_buffer(chime_object_t* instance)
{
    return ((chime_string_t*)instance)->buffer;
}

void string_set_buffer(chime_object_t* instance, char* value)
{
    char* buffer;
    
    buffer = string_get_buffer(instance);
    if (buffer != STRING_EMPTY_BUFFER_FLAG)
        chime_deallocate(buffer);
    
    ((chime_string_t*)instance)->buffer = value;
}
