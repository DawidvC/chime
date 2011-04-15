// Chime Runtime: chime_dictionary.h

#ifndef CHIME_DICTIONARY
#define CHIME_DICTIONARY

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _chime_dictionary chime_dictionary_t;

chime_dictionary_t* chime_dictionary_create(void);
void                chime_dictionary_destroy(chime_dictionary_t* dictionary);

void* chime_dictionary_get(chime_dictionary_t* dictionary, const char* key);
void  chime_dictionary_set(chime_dictionary_t* dictionary, const char* key, void* value);

#ifdef __cplusplus
}
#endif

#endif