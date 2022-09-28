#include "list.h"

List* list_create()
{
	List* list;
	if ((list = (List*) malloc (sizeof(List))) == NULL) 
        	return NULL;

   	list-> m_head.m_next = &list-> m_tail;
  	list-> m_head.m_prev = &list-> m_head;
  	list-> m_tail.m_next = &list-> m_tail;
	list-> m_tail.m_prev = &list-> m_head;
	list->m_size = 0;
 	return list;
}

void list_destroy(List** list, ElementDestroy free_item)
{
	Node* temp;
	Node* next;

	if(list == NULL || *list == NULL)
		return;
	
	temp = (*list) -> m_head.m_next;
   	while (temp != &(*list)-> m_tail)
	{
		if(free_item != NULL)
			free_item(temp -> m_data);		

		next = temp -> m_next;
		free(temp);
		temp = next;   		
	}

	free(*list);
	*list = NULL;
}

void list_erase(const List* list, ElementDestroy free_item)
{
	if(list == NULL)
		return;

	Node* next;
	Node* it = list->m_head.m_next;
   	while (it != &list->m_tail)
	{
		next = it->m_next;

		if(free_item != NULL)
			free_item(it->m_data);
		pop_node(it);	

		it = next;  
	}
}

List_return list_push_head(List* list, void* item)
{
	Node *node = NULL;

	if(list == NULL || item == NULL)
		return LIST_UNINITIALIZED_ERROR;
	
	if((node = (Node*)malloc(sizeof(Node))) == NULL)
		return LIST_ALLOCATION_ERROR;

	node->m_data = item;
	push_node(node,list->m_head.m_next);
	list->m_size++;
	return LIST_SUCCESS;
}

List_return list_push_tail(List* list, void* item)
{
	Node* node = NULL;

	if(list == NULL || item == NULL)
		return LIST_UNINITIALIZED_ERROR;

	if((node = (Node*) malloc(sizeof(Node))) == NULL)
		return LIST_ALLOCATION_ERROR;

	node->m_data = item;
	push_node(node, &list->m_tail);
	list->m_size++;
	return LIST_SUCCESS;
}

List_return list_pop_head(List* list, void** item)
{
	if (list == NULL ||  item == NULL) 
      	return LIST_UNINITIALIZED_ERROR;
   		
	if(list -> m_head.m_next == &list -> m_tail)
		return LIST_UNDERFLOW_ERROR;
	
	*item = list->m_head.m_next->m_data;
	pop_node (list->m_head.m_next); 
	list->m_size--;
	return LIST_SUCCESS;
}

List_return list_pop_tail(List* list, void** item)
{
	if (list == NULL || item == NULL) 
      	return LIST_UNINITIALIZED_ERROR;
   	
	if(list->m_head.m_next == &list->m_tail)
		return LIST_UNDERFLOW_ERROR;
	
	*item = list->m_tail.m_prev->m_data;
	pop_node(list->m_tail.m_prev);
	list->m_size--;	
	return LIST_SUCCESS;
}

size_t list_size(const List* list)
{
	return list->m_size;
}

int list_is_exists(const List* list, Compar is_equal, const void* item)
{
	if(list == NULL || is_equal == NULL || item == NULL)
		return LIST_NOT_INITIALIZED;

	Node* next;
	Node* it = list->m_head.m_next;
   	while (it != &list->m_tail)
	{
		next = it->m_next;

		if(is_equal(it->m_data, item) == YES)
			return YES;	

		it = next;  
	}

	return NO;
}

void push_node(Node *node, Node *next)
{
	node->m_prev = next->m_prev;
	node->m_next = next;
	next->m_prev->m_next = node;
	next->m_prev = node;
}

void pop_node(Node* node)
{
	node->m_next->m_prev = node->m_prev;
	node->m_prev->m_next = node->m_next;
	free(node);
}

void list_print(List* list, PrintItem print)
{
	int i = 1;
	Node *node;
	void* item;

	if (list == NULL || print == NULL) 
        return;
	
	node =list->m_head.m_next;
	while(node != &list->m_tail) 
	{
		item = node->m_data;
		print(item);
		node = node->m_next; 
	} 
}
