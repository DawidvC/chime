// Chime Runtime: chime_string_methods.c

#include "chime_string_methods.h"
#include "runtime/string/chime_string.h"
#include "runtime/literals/chime_literal.h"

#include <stdio.h>
#include <string.h>

chime_object_t* string_to_string(chime_object_t* instance)
{
    return instance;
}

chime_object_t* string_print(chime_object_t* instance)
{
    fprintf(stdout, "<String: %s>\n", chime_string_to_c_string(instance));
    
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
