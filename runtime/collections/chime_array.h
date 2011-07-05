// Chime Runtime: chime_array.h

#ifndef CHIME_ARRAY
#define CHIME_ARRAY

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _chime_array chime_array_t;

chime_array_t* chime_array_create(void);
void           chime_array_destroy(chime_array_t* array);

unsigned long chime_array_count(chime_array_t* array);

void* chime_array_get(chime_array_t* array, unsigned long index);
void  chime_array_add(chime_array_t* array, void* value);
void  chime_array_remove(chime_array_t* array, unsigned long index);

#ifdef __cplusplus
}
#endif

#endif
