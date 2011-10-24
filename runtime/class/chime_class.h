// Chime Runtime: chime_class.h

#ifndef CHIME_CLASS_H
#define CHIME_CLASS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _chime_class chime_class_t;

chime_class_t*  chime_class_create_object_subclass(const char* name);
chime_class_t*  chime_class_create(const char* name, chime_class_t* superclass);
void            chime_class_destroy(chime_class_t* klass);

chime_class_t*  chime_class_get_class(chime_class_t* klass);
chime_class_t*  chime_class_get_superclass(chime_class_t* klass);
char*           chime_class_get_name(chime_class_t* klass);

void            chime_class_set_instance_method(chime_class_t* klass, const char* name, void* function);
void            chime_class_set_class_method(chime_class_t* klass, const char* name, void* function);
void            chime_class_include_trait(chime_class_t* klass, chime_class_t* trait);

#ifdef __cplusplus
}
#endif

#endif // CHIME_CLASS_H
