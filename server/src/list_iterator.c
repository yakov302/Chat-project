#include "list_iterator.h"

ListItr begin(const List* list)
{
	if(list == NULL)
		return NULL;
	
	return list->m_head.m_next;
}

ListItr end(const List* list)
{
	if(list == NULL)
		return NULL;
	
	return (ListItr*)&list-> m_tail;
}

int is_equals(const ListItr a, const ListItr b)
{
	if(a == NULL || b == NULL)
		return -1;
	
	if(a == b)
		return 1;
	
	return 0;
}

ListItr next(ListItr itr)
{
	if(itr == NULL)
		return NULL;
	
	return ((Node*)itr)->m_next;
}

ListItr prev(ListItr itr)
{
	if(itr == NULL)
		return NULL;

	if(((Node*)itr)->m_prev->m_prev == ((Node*)itr)->m_prev)
		return itr;	
	
	return ((Node*)itr)->m_prev;
}

void* get_data(ListItr itr)
{
	if(itr == NULL || itr == next(itr))
		return NULL;

	return ((Node*)itr)->m_data;
}

void* set_data(ListItr itr, void* element)
{
	void* old_element;

	if(itr == NULL || element == NULL || itr == next(itr))
		return NULL;
	
	old_element = ((Node*)itr)->m_data;
	((Node*)itr)->m_data = element;
	return old_element;
}

ListItr insert_before(List* list, ListItr itr, void* element)
{
	Node *node = NULL;

	if(itr == NULL || element == NULL) 
		return NULL;

	if((node = (Node*)malloc(sizeof(Node))) == NULL)
		return NULL;	

	push_node(node, itr);
	list->m_size++;
	node->m_data = element;
	return node;
}

void* remove_it(List* list, ListItr itr)
{
	void* deleted_node_data;

	if(itr == NULL || itr == next(itr))
		return NULL;

	deleted_node_data = ((Node*)itr)->m_data;
	pop_node(itr);
	list->m_size--;	
	return deleted_node_data;
}

ListItr find_first(const List* list, Compar is_equal, const void* item)
{
	if(list == NULL || is_equal == NULL || item == NULL)
		return NULL;

	ListItr it = begin(list);
	ListItr end_it = end(list);
	while(it != end_it)
	{
		if(is_equal(get_data(it), item) == YES)
			return it;
		
		it = next(it);
	}
	
	return NULL;
}

static void push_node(Node* node, Node* next)
{
	node->m_prev = next->m_prev;
	node->m_next = next;
	next->m_prev->m_next = node;
	next->m_prev = node;
}

static void pop_node(Node* node)
{
	node->m_next->m_prev = node->m_prev;
	node->m_prev->m_next = node->m_next;
	free(node);
}


