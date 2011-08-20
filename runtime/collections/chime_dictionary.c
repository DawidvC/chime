// Chime Runtime: chime_dictionary.c

#include "chime_dictionary.h"
#include "chime_dictionary_internal.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

chime_dictionary_t* chime_dictionary_create(void)
{
    chime_dictionary_t* dictionary;
    
    dictionary = (chime_dictionary_t*)malloc(sizeof(chime_dictionary_t));
    
    dictionary->key_array   = chime_runtime_array_create();
    dictionary->value_array = chime_runtime_array_create();
    
    assert(dictionary);
    
    return dictionary;
}

void chime_dictionary_destroy(chime_dictionary_t* dictionary)
{
    assert(dictionary);
    
    chime_runtime_array_destroy(dictionary->key_array);
    chime_runtime_array_destroy(dictionary->value_array);
    
    free(dictionary);
}

unsigned long chime_dictionary_count(chime_dictionary_t* dictionary)
{
    assert(dictionary);
    
    return chime_runtime_array_count(dictionary->key_array);
}

void* chime_dictionary_get(chime_dictionary_t* dictionary, const char* key)
{
    unsigned long i;
    char*         current_key;
    
    for (i = 0; i < chime_dictionary_count(dictionary); ++i)
    {
        current_key = chime_runtime_array_get(dictionary->key_array, i);
        
        // strcmp is bad
        if (strcmp(current_key, key) == 0)
        {
            return chime_runtime_array_get(dictionary->value_array, i);
        }
    }
    
    return 0;
}

void chime_dictionary_set(chime_dictionary_t* dictionary, const char* key, void* value)
{
    assert(dictionary);
    
    chime_dictionary_remove(dictionary, key);
    
    chime_runtime_array_add(dictionary->key_array,   (void*)key);
    chime_runtime_array_add(dictionary->value_array, value);
    
    assert(chime_runtime_array_count(dictionary->key_array) == chime_runtime_array_count(dictionary->value_array));
}

void chime_dictionary_remove(chime_dictionary_t* dictionary, const char* key)
{
    unsigned long i;
    char*         current_key;
    
    assert(dictionary);
    
    for (i = 0; i < chime_dictionary_count(dictionary); ++i)
    {
        current_key = chime_runtime_array_get(dictionary->key_array, i);
        
        // strcmp is bad
        if (strcmp(current_key, key) == 0)
        {
            chime_runtime_array_remove(dictionary->key_array, i);
            chime_runtime_array_remove(dictionary->value_array, i);
            
            break;
        }
    }
    
    assert(chime_runtime_array_count(dictionary->key_array) == chime_runtime_array_count(dictionary->value_array));
}

char* chime_dictionary_get_key(chime_dictionary_t* dictionary, void* value)
{
    unsigned long i;
    char*         current_value;
    
    for (i = 0; i < chime_dictionary_count(dictionary); ++i)
    {
        current_value = chime_runtime_array_get(dictionary->value_array, i);
        
        if (current_value == value)
        {
            return chime_runtime_array_get(dictionary->key_array, i);
        }
    }
    
    return 0;
}
