// Chime Runtime: chime_array.c

#include "chime_array.h"
#include "chime_array_internal.h"
#include <stdlib.h>
#include <assert.h>

chime_array_t* chime_array_create(void)
{
    chime_array_t* array;
    
    array = (chime_array_t*)malloc(sizeof(chime_array_t));
    array->count = 0;
    array->head  = 0;
    
    return array;
}

void chime_array_destroy(chime_array_t* array)
{
    unsigned long index;
    
    assert(array);
    
    index = chime_array_count(array);
    
    while (index)
    {
        chime_array_remove(array, index-1);
        index--;
    }
    
    free(array);
}

chime_array_node_t* chime_array_node_create(void* p)
{
    chime_array_node_t* node;
    
    node = (chime_array_node_t*)malloc(sizeof(chime_array_node_t));
    node->next = 0;
    node->pointer = p;
    
    return node;
}

unsigned long chime_array_count(chime_array_t* array)
{
    assert(array);
    
    return array->count;
}

void* chime_array_get(chime_array_t* array, unsigned long index)
{
    chime_array_node_t* node;
    unsigned long       i;
    
    assert(array);
    assert(index < chime_array_count(array));
    
    node = array->head;
    
    for (i = 0; i < index; i++)
    {
        node = node->next;
    }
    
    return node->pointer;
}

void chime_array_add(chime_array_t* array, void* value)
{
    chime_array_node_t* new_node;
    chime_array_node_t* node;
    
    assert(array);
    
    // make our new node and increase the array's account
    new_node = chime_array_node_create(value);
    
    array->count++;
    
    // get a reference to the head node
    node = array->head;
    
    // handle the special-case of no items in the array
    if (node == 0)
    {
        array->head = new_node;
        return;
    }
    
    // walk the list until we find the last node
    while (node->next != 0)
    {
        node = node->next;
    }
    
    node->next = new_node;
}

void* chime_array_remove(chime_array_t* array, unsigned long index)
{
    return 0;
}
