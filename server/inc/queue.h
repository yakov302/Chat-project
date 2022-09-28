#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef void(*DestroyItem)(void* element);
typedef int(*ActionFunction)(const void* element, void* context);

typedef struct Queue
{
	void** m_queue;        
	size_t m_size;         
	size_t m_in;           
	size_t m_out;            
	size_t m_num_of_item; 
    DestroyItem m_item_destroy;           

}Queue;

typedef enum
{
    QUEUE_SUCCESS,
    QUEUE_UNINITIALIZED_ARGS,
    QUEUE_FULL,
    QUEUE_EMPTY,

}Queue_return;

Queue* queue_create(size_t capacity, DestroyItem destroy_item);

void queue_destroy(Queue** queue);

Queue_return queue_insert(Queue* queue, void* item);

Queue_return queue_remove(Queue* queue, void** deleted_item);

Queue_return queue_remove_and_free(Queue* queue);

int queue_is_empty(Queue* queue);

int queue_size(Queue* queue);

size_t queue_for_each(Queue* queue, ActionFunction action , void* context);


#endif // QUEUE_H