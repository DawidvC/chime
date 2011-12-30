#include "chime_string_extensions.h"
#include "runtime/classes/integer/chime_integer.h"
#include "runtime/classes/string/chime_string.h"
#include "runtime/platform.h"
#include "library/data/chime_data.h"

#include <stdio.h>

chime_object_t* string_to_data(chime_object_t* instance)
{
    chime_object_t* data_object;
    signed long     string_length;
    char*           buffer;
    char*           c_string;
    
    data_object = chime_class_instantiate_named("Data");
    
    // what we need to do is walk through the string, character by character,
    // and make a byte-for-byte copy into this data object
    
    string_length = chime_integer_decode(chime_object_invoke_0(instance, "length"));
    
    // allocate enough space for the string
    buffer = chime_malloc(string_length);
    
    // this is horribly inefficient
    c_string = chime_string_to_c_string(instance);
    chime_memcpy(buffer, c_string, string_length);
    
    data_append_raw_block(data_object, buffer, string_length);
    
    return data_object;
}
