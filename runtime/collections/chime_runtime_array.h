// Chime Runtime: chime_runtime_array.h

#ifndef chime_runtime_array
#define chime_runtime_array

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _chime_runtime_array chime_runtime_array_t;

chime_runtime_array_t* chime_runtime_array_create(void);
void           chime_runtime_array_destroy(chime_runtime_array_t* array);

unsigned long chime_runtime_array_count(chime_runtime_array_t* array);

void* chime_runtime_array_get(chime_runtime_array_t* array, unsigned long index);
void  chime_runtime_array_add(chime_runtime_array_t* array, void* value);
void  chime_runtime_array_remove(chime_runtime_array_t* array, unsigned long index);

#ifdef __cplusplus
}
#endif

#endif
