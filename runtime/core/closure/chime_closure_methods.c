#include "chime_closure_methods.h"
#include "chime_closure.h"
#include "runtime/classes/string/chime_string.h"

#include <stdio.h>
#include <assert.h>

chime_object_t* closure_to_string(chime_object_t* instance)
{
    return chime_string_create_with_c_string("<Closure: some more details>", 29);
}
