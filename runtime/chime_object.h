// Chime Runtime: chime_object.h

#ifndef CHIME_OBJECT
#define CHIME_OBJECT

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _chime_object chime_object_t;

typedef chime_object_t*	(*chime_function_t)(chime_object_t*, const char*, ...);

chime_object_t* chime_object_create(chime_object_t* object_class);
chime_object_t* chime_object_create_with_name(const char* class_name);

void            chime_object_destroy(chime_object_t* instance);

chime_object_t* chime_object_get_class(chime_object_t* instance);
chime_object_t* chime_object_get_property(chime_object_t* instance, const char* name);
void            chime_object_set_property(chime_object_t* instance, const char* name, chime_object_t* value);

chime_function_t chime_object_get_function(chime_object_t* instance);
void             chime_object_set_function(chime_object_t* instance, const char* name, chime_function_t function, unsigned long arity);

chime_object_t* chime_object_invoke(chime_object_t* instance, const char* name, ...);

#ifdef __cplusplus
}
#endif

#endif
