// Chime Runtime: chime_dictionary_internal.h

#ifndef CHIME_DICTIONARY_INTERNAL
#define CHIME_DICTIONARY_INTERNAL

#include "runtime/collections/chime_array.h"

// struct _chime_binary_tree_node {
//     struct _chime_binary_tree_node* left;
//     struct _chime_binary_tree_node* right;
//     
//     void* pointer;
// };
// 
// typedef struct _chime_binary_tree_node chime_binary_tree_node_t;

struct _chime_dictionary {    
    chime_array_t* key_array;
    chime_array_t* value_array;
};

#endif
