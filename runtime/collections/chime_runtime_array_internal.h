// Chime Runtime: chime_runtime_array_internal.h

#ifndef chime_runtime_array_INTERNAL
#define chime_runtime_array_INTERNAL

struct _chime_runtime_array_node {
    struct _chime_runtime_array_node* next;
    
    void* pointer;
};

typedef struct _chime_runtime_array_node chime_runtime_array_node_t;

struct _chime_runtime_array {
    unsigned long count;
    
    chime_runtime_array_node_t* head;
};

chime_runtime_array_node_t* chime_runtime_array_node_create(void* p);
void                        chime_runtime_array_node_destroy(chime_runtime_array_node_t* node);

#endif
