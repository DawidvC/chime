// Chime Runtime: chime_array_internal.h

#ifndef CHIME_ARRAY_INTERNAL
#define CHIME_ARRAY_INTERNAL

struct _chime_array_node {
    struct _chime_array_node* next;
    
    void* pointer;
};

typedef struct _chime_array_node chime_array_node_t;

struct _chime_array {
    unsigned long count;
    
    chime_array_node_t* head;
};

chime_array_node_t* chime_array_node_create(void* p);
void                chime_array_node_destroy(chime_array_node_t* node);

#endif
