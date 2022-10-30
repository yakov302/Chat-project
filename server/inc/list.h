#ifndef LIST_H
#define LIST_H

#include <stddef.h>  
#include <stdio.h>
#include <stdlib.h>

#define YES 1
#define NO 0

typedef struct Node
{
    void* m_data;
    struct Node *m_next;
    struct Node *m_prev;

}Node;

typedef struct List
{
    Node m_head;
    Node m_tail;
    size_t m_size;

}List;

typedef enum List_return
{
    LIST_SUCCESS,
    LIST_UNINITIALIZED_ERROR,			
    LIST_ALLOCATION_ERROR,				
    LIST_NULL_ELEMENT_ERROR,
    LIST_UNDERFLOW_ERROR,
    LIST_NOT_INITIALIZED

} List_return;

typedef int (*Compar)(const void*, const void*);
typedef void (*ElementDestroy)(void*);
typedef void(*PrintItem)(const void*);

static void push_node(Node* node, Node* next);
static void pop_node(Node* node);

List* list_create(void);

void list_destroy(List** _list, ElementDestroy element_destroy);

void list_erase(const List* list, ElementDestroy element_destroy);

List_return list_push_head(List* _list, void* item);

List_return list_push_tail(List* _list, void* item);

List_return list_pop_head(List* _list, void** deleted_item);

List_return list_pop_tail(List* _list, void** deleted_item);

size_t list_size(const List* _list);

void list_print(List* list, PrintItem print);

int list_is_exists(const List* list, Compar is_equal, const void* item);


#endif// LIST_H

