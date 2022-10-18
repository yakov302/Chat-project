#include "queue.h"

Queue* queue_create(size_t capacity, DestroyItem destroy_item)
{	
	if(capacity < 1)
		return NULL;
	
    Queue* queue = (Queue*)malloc(sizeof(Queue));
	if(queue == NULL)
		return NULL;

    queue->m_queue = (void**)malloc(capacity*sizeof(void*));
	if(queue->m_queue == NULL)
	{
		free(queue);
		return NULL;
	}

	queue->m_size = capacity;
	queue->m_in = 0;
	queue->m_out = 0;
	queue->m_num_of_item = 0;
    queue->m_item_destroy = destroy_item;
    return queue;
}

static void destroy_items(Queue* queue)
{
    int j = queue->m_out;	
    for(int i = 0; i < queue->m_num_of_item ; ++i)
    {			
        queue->m_item_destroy(queue->m_queue[j]);
        j = (j+1)%(queue->m_size);
    }
}

void queue_destroy(Queue** queue)
{
	if(queue == NULL || *queue == NULL)
		return;

	if((*queue)->m_item_destroy != NULL)
        destroy_items(*queue);
	
	free ((*queue)->m_queue);
	free(*queue);
	*queue = NULL;
}

Queue_return queue_insert(Queue* queue, void* item)
{
	if(queue == NULL || item == NULL)
		return QUEUE_UNINITIALIZED_ARGS;
	
	if(queue->m_num_of_item == queue->m_size)
		return QUEUE_FULL;
	
	queue->m_queue[queue->m_in] = item;
	queue->m_in = (queue->m_in + 1)%(queue->m_size);
	queue->m_num_of_item++;		
    return QUEUE_SUCCESS;
}

Queue_return queue_remove(Queue* queue, void** deleted_item)
{
    if(queue == NULL || deleted_item == NULL)
		return QUEUE_UNINITIALIZED_ARGS;
	
	if (queue->m_num_of_item == 0)
		return QUEUE_EMPTY;
	
	*deleted_item = queue->m_queue[queue->m_out];
	queue->m_out = (queue->m_out + 1)%(queue->m_size);
	queue->m_num_of_item--;
    return QUEUE_SUCCESS;
}

Queue_return queue_remove_and_free(Queue* queue)
{
    if(queue == NULL)
		return QUEUE_UNINITIALIZED_ARGS;
	
	if (queue->m_num_of_item == 0)
		return QUEUE_EMPTY;
		
	queue->m_item_destroy(queue->m_queue[queue->m_out]);
	queue->m_out = (queue->m_out + 1)%(queue->m_size);
	queue->m_num_of_item--;
    return QUEUE_SUCCESS;
}

int queue_is_empty(Queue* queue)
{
    if (queue == NULL)
 	 	return QUEUE_UNINITIALIZED_ARGS;

 	if (queue->m_num_of_item == 0)
  		return TRUE;

    return FALSE;
}

int queue_size(Queue* queue)
{
    if (queue == NULL)
 	 	return QUEUE_UNINITIALIZED_ARGS;

    return queue->m_num_of_item;
}

size_t queue_for_each(Queue* queue, ActionFunction action , void* context)
{
	if(queue == NULL || action == NULL)
		return -1;

    int i;
    int j = queue->m_out;	
    for(i = 0; i < queue->m_num_of_item ; ++i)
    {			
        if(action(queue->m_queue[j], context) == FALSE)
            break;

        j = (j+1)%(queue->m_size);
    }

    return i;
}
