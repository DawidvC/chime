// Chime Runtime: chime_object.h

#ifndef CHIME_OBJECT
#define CHIME_OBJECT

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _chime_object chime_object_t;

typedef chime_object_t*	(*chime_function_t)(chime_object_t*, const char*, ...);

typedef enum {
    CHIME_OBJECT_INVALID_TYPE = 0,
    CHIME_OBJECT_TYPE         = 1,
    CHIME_NULL_TYPE           = 2,
    CHIME_INTEGER_TYPE        = 3,
    CHIME_BOOLEAN_TYPE        = 4
} chime_object_type_t;

chime_object_t* chime_object_create(chime_object_t* object_class);
chime_object_t* chime_object_create_with_name(const char* class_name);

void            chime_object_destroy(chime_object_t* instance);

chime_object_type_t chime_object_get_type(chime_object_t* instance);

chime_object_t* chime_object_get_class(chime_object_t* instance);
chime_object_t* chime_object_get_metaclass(chime_object_t* instance);
chime_object_t* chime_object_get_property(chime_object_t* instance, const char* name);
void            chime_object_set_property(chime_object_t* instance, const char* name, chime_object_t* value);

chime_function_t chime_object_get_function(chime_object_t* instance);
void             chime_object_set_function(chime_object_t* instance, const char* name, chime_function_t function, unsigned long arity);

chime_object_t* chime_object_invoke(chime_object_t* instance, const char* name, ...);

#ifdef __cplusplus
}
#endif

#endif
