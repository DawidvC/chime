// Chime Runtime: chime_literal_methods.h

#ifndef CHIME_LITERAL_METHODS_H
#define CHIME_LITERAL_METHODS_H

#include "runtime/object/chime_object.h"

#ifdef __cplusplus
extern "C" {
#endif

chime_object_t* null_print(chime_object_t* instance);

chime_object_t* integer_print(chime_object_t* instance);
chime_object_t* integer_times(chime_object_t* instance, chime_object_t* function);
chime_object_t* integer_add(chime_object_t* instance, chime_object_t* other);
chime_object_t* integer_multiply(chime_object_t* instance, chime_object_t* other);
chime_object_t* integer_compare(chime_object_t* instance, chime_object_t* other);

chime_object_t* boolean_print(chime_object_t* instance);
chime_object_t* boolean_compare(chime_object_t* instance, chime_object_t* other);

#ifdef __cplusplus
}
#endif

#endif // CHIME_LITERAL_METHODS_H
