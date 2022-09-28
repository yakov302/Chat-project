#ifndef LIST_ITERATOR_H
#define LIST_ITERATOR_H

#include "list.h"

typedef void* ListItr;

ListItr begin(const List* list);

ListItr end(const List* list);

int is_equals(const ListItr a, const ListItr b);

ListItr next(ListItr itr);

ListItr prev(ListItr itr);

void* get_data(ListItr itr);

void* set_data(ListItr itr, void* element);

ListItr insert_before(List* list, ListItr itr, void* element);

void* remove_it(List* list, ListItr itr);

ListItr find_first(const List* list, Compar is_equal, const void* item);


#endif // LIST_ITERATOR_H