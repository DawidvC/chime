// Chime Runtime: chime_runtime_internal.h

#ifndef CHIME_RUNTIME_INTERNAL
#define CHIME_RUNTIME_INTERNAL

#include "runtime/collections/chime_dictionary.h"
#include "runtime/class/chime_class.h"

extern chime_dictionary_t* _chime_modules;
extern chime_dictionary_t* _chime_classes;
extern chime_dictionary_t* _chime_traits;

extern chime_class_t*      _object_class;
extern chime_class_t*      _method_class;
extern chime_class_t*      _trait_class;
extern chime_class_t*      _module_class;
extern chime_class_t*      _reference_class;
extern chime_class_t*      _closure_class;

extern chime_class_t*      _string_class;
extern chime_class_t*      _array_class;
extern chime_class_t*      _hash_class;
extern chime_class_t*      _range_class;
extern chime_class_t*      _float_class;

extern chime_class_t*      _comparable_trait;

#endif
