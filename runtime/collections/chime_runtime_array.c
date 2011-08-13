// Chime Runtime: chime_runtime_array.c

#include "chime_runtime_array.h"
#include "chime_runtime_array_internal.h"

#include <stdlib.h>
#include <assert.h>

chime_runtime_array_t* chime_runtime_array_create(void)
{
    chime_runtime_array_t* array;
    
    array = (chime_runtime_array_t*)malloc(sizeof(chime_runtime_array_t));
    array->count = 0;
    array->head  = 0;
    
    return array;
}

void chime_runtime_array_destroy(chime_runtime_array_t* array)
{
    unsigned long index;
    
    assert(array);
    
    index = chime_runtime_array_count(array);
    
    while (index)
    {
        chime_runtime_array_remove(array, index-1);
        --index;
    }
    
    free(array);
}

chime_runtime_array_node_t* chime_runtime_array_node_create(void* p)
{
    chime_runtime_array_node_t* node;
    
    node = (chime_runtime_array_node_t*)malloc(sizeof(chime_runtime_array_node_t));
    node->next = 0;
    node->pointer = p;
    
    return node;
}

void chime_runtime_array_node_destroy(chime_runtime_array_node_t* node)
{
    assert(node);
    
    free(node);
}

unsigned long chime_runtime_array_count(chime_runtime_array_t* array)
{
    assert(array);
    
    return array->count;
}

void* chime_runtime_array_get(chime_runtime_array_t* array, unsigned long index)
{
    chime_runtime_array_node_t* node;
    unsigned long       i;
    
    assert(array);
    assert(index < chime_runtime_array_count(array));
    
    node = array->head;
    
    for (i = 0; i < index; ++i)
    {
        node = node->next;
    }
    
    assert(node);
    
    return node->pointer;
}

void chime_runtime_array_add(chime_runtime_array_t* array, void* value)
{
    chime_runtime_array_node_t* new_node;
    chime_runtime_array_node_t* node;
    
    assert(array);
    
    // make our new node and increase the array's account
    new_node = chime_runtime_array_node_create(value);
    assert(new_node);
    
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

void chime_runtime_array_remove(chime_runtime_array_t* array, unsigned long index)
{
    chime_runtime_array_node_t* node;
    chime_runtime_array_node_t* prev_node;
    unsigned long               i;
    
    assert(array);
    assert(index < chime_runtime_array_count(array));
    
    node = array->head;
    
    array->count--; // decrease the count, this is an important step!
    
    if (index == 0)
    {
        array->head = array->head->next;
        
        chime_runtime_array_node_destroy(node);
        
        return;
    }
    
    do
    {
        prev_node = node;
        node      = node->next;
        
        ++i;
    } while (node->next && (i < index - 1));
    
    prev_node->next = node->next;
    
    chime_runtime_array_node_destroy(node);
}
