// Chime Runtime: chime_string_methods.c

#include "chime_string_methods.h"
#include "runtime/string/chime_string.h"
#include "runtime/literals/chime_literal.h"
#include "runtime/support.h"
#include "runtime/chime_runtime_internal.h"
#include "runtime/collections/chime_runtime_array.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

chime_object_t* string_class_new(chime_class_t* klass)
{
    chime_object_t*        string_instance;
    chime_runtime_array_t* internal_array;
    
    // first thing to do is invoke new on super
    string_instance = chime_object_create(klass);
    internal_array  = chime_runtime_array_create();
    
    chime_object_set_attribute(string_instance, "_internal_array", (chime_object_t*)internal_array);
    chime_object_set_attribute(string_instance, "_length", chime_literal_encode_integer(0));
    chime_object_set_attribute(string_instance, "_c_string", (chime_object_t*)0);
    
    return string_instance;
}

chime_object_t* string_to_string(chime_object_t* instance)
{
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
    
    result = strcmp(s1, s2);
    
    if (result > 0)
        return chime_literal_encode_integer(1);
    
    if (result < 0)
        return chime_literal_encode_integer(-1);
    
    return chime_literal_encode_integer(0);
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
        
        start  = chime_literal_decode_integer(chime_object_invoke_0(index, "start"));
        end    = chime_literal_decode_integer(chime_object_invoke_0(index, "end"));
        length = chime_literal_decode_integer(chime_object_invoke_0(instance, "length"));
        
        if (end < 0)
        {
            // because end is negative, we add it and because the length-1
            // equals the last character we don't need to make any other adjustments
            end += length;
        }
        
        assert(start < length && "Range.start must be < string length");
        
        c_string = chime_string_to_c_string(instance);
        
        c_string += start; // a little bit of pointer arithmatic here
        
        return chime_string_create_with_c_string(c_string, end - start + 1);
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
    return chime_object_get_attribute(instance, "_length");
}

chime_object_t* string_concatenate(chime_object_t* instance, chime_object_t* other)
{
    return instance;
}

chime_runtime_array_t* string_get_internal_array(chime_object_t* instance)
{
    return (chime_runtime_array_t*)chime_object_get_attribute(instance, "_internal_array");
}
