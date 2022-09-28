#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>

#define REDUCTION_CRITERION 2
#define TRUE 1
#define FALSE 0

typedef struct Vector
{
    void** m_array;
    size_t m_original_size;
    size_t m_size;
    size_t m_nun_of_items;
    size_t m_resizing_size ;

}Vector;

typedef enum Vector_return 
{
    VECTOR_SUCCESS,
    VECTOR_UNITIALIZED_ARGS, 
    VECTOR_MALLOC_FAIL,
    VECTOR_REALLOC_FAIL,
    VECTOR_INVALID_INDEX,
    VECTOR_FULL,
    VECTOR_EMPTY

}Vector_return;

typedef void(*ElementDestroy)(void* item);
typedef int(*Action)(void*, void*);

Vector* vector_create (size_t capacity, size_t resizing_size);

void vector_destroy (Vector** vector, ElementDestroy element_destroy);

Vector_return vector_push_back(Vector* vector, void* item);

Vector_return vector_pop_back(Vector* vector, void** return_item);

Vector_return vector_pop_back_no_return(Vector* vector);

Vector_return vector_pop(Vector* vector, size_t index, void** return_item); // o(n) to shrink

Vector_return vector_pop_no_return(Vector* vector, size_t index); // o(n) to shrink

Vector_return vector_get(const Vector* vector, size_t index, void** return_item);

Vector_return vector_set(Vector* vector, size_t index, void* item); 

size_t vector_capacity (const Vector* vector);

size_t vector_size (const Vector* vector);

int vector_empty(const Vector* vector);

void vector_clear(Vector* vector);

size_t vector_for_each(const Vector* vector, Action action_function, void* context);


#endif // VECTOR_H
