#include "vector.h"

Vector* vector_create (size_t capacity, size_t resizing_size)
{
	if(capacity < 1 && resizing_size < 1) {return NULL;}

	Vector* vector = (Vector*)malloc(sizeof(Vector));
	if(vector == NULL){return NULL;}

    vector->m_array = (void**)malloc(capacity*sizeof(void*));
	if (vector->m_array == NULL)
	{
		free(vector);
		return NULL;
	}

	vector->m_original_size = capacity;
	vector->m_size = capacity;
	vector->m_nun_of_items = 0;
	vector->m_resizing_size = resizing_size;
    return vector;
}

void vector_destroy (Vector** vector, ElementDestroy element_destroy)
{
	if(vector == NULL || *vector == NULL) {return;}
	
	if(element_destroy != NULL)
	{
        size_t size = (*vector)->m_nun_of_items;
		for(size_t i = 0; i < size; ++i)
			element_destroy((*vector)->m_array[i]);
	}
	
	free ((*vector)->m_array);
	free(*vector);
	*vector = NULL;
}

static int need_to_increase_capacity(Vector* vector)
{
    return vector->m_nun_of_items >= vector->m_size;
}

static Vector_return vector_increase_capacity(Vector* vector)
{
    if(vector->m_resizing_size < 1)
		return VECTOR_FULL;

	void** temp = (void**)realloc(vector->m_array, (vector->m_size + vector->m_resizing_size)*sizeof(void*));
	if(temp == NULL)
		return VECTOR_REALLOC_FAIL;

	vector->m_array = temp;
	vector->m_size += vector->m_resizing_size;	
    return VECTOR_SUCCESS;
}

Vector_return vector_push_back(Vector* vector, void* item)
{
	if(vector == NULL || item == NULL) {return VECTOR_UNITIALIZED_ARGS;}

	if(need_to_increase_capacity(vector))
	{
		Vector_return result = vector_increase_capacity(vector);
        if(result != VECTOR_SUCCESS)
			return result;
	}
	
	vector->m_array[vector->m_nun_of_items] = item;
	vector->m_nun_of_items++;	
    return VECTOR_SUCCESS;
}

static int need_to_decrease_capacity(Vector* vector)
{
    return (vector->m_size - vector->m_nun_of_items) >= (vector->m_resizing_size * REDUCTION_CRITERION)
        && (vector->m_size - vector->m_resizing_size) >= (vector-> m_original_size);
}

static Vector_return vector_decrease_capacity(Vector* vector)
{
	void** temp = (void**)realloc(vector->m_array, (vector->m_size - vector->m_resizing_size)*sizeof(void*));
	if(temp == NULL)
		return VECTOR_REALLOC_FAIL;

	vector->m_array = temp;
	vector->m_size -= vector->m_resizing_size;	
    return VECTOR_SUCCESS;
}

Vector_return vector_pop_back(Vector* vector, void** return_item)
{
	if(vector == NULL || return_item == NULL) {return VECTOR_UNITIALIZED_ARGS;}

	if (vector->m_nun_of_items < 1)
		return VECTOR_EMPTY;

	if(need_to_decrease_capacity(vector))
	{
		Vector_return result = vector_decrease_capacity(vector);
        if(result != VECTOR_SUCCESS)
			return result;
	}

	*return_item =  vector->m_array[vector->m_nun_of_items - 1];
	vector->m_nun_of_items--;
    return VECTOR_SUCCESS;
}

Vector_return vector_pop_back_no_return(Vector* vector)
{
    if(vector == NULL) {return VECTOR_UNITIALIZED_ARGS;}

	if (vector->m_nun_of_items < 1)
		return VECTOR_EMPTY;

	if(need_to_decrease_capacity(vector))
	{
		Vector_return result = vector_decrease_capacity(vector);
        if(result != VECTOR_SUCCESS)
			return result;
	}

	vector->m_nun_of_items--;
    return VECTOR_SUCCESS;
}

static void shrink_array(Vector* vector, size_t index)
{
	const size_t size = vector->m_nun_of_items;
	
	for(size_t i = index + 1; i < size; ++i)
		vector->m_array[i-1] = vector->m_array[i];
}

Vector_return vector_pop(Vector* vector, size_t index, void** return_item)
{
	if(vector == NULL || return_item == NULL) {return VECTOR_UNITIALIZED_ARGS;}
	if (index < 0 || index >= vector->m_nun_of_items) {return VECTOR_INVALID_INDEX;}

	if(need_to_decrease_capacity(vector))
	{
		Vector_return result = vector_decrease_capacity(vector);
        if(result != VECTOR_SUCCESS)
			return result;
	}

	*return_item =  vector->m_array[index];
	shrink_array(vector, index);
	vector->m_nun_of_items--;
    return VECTOR_SUCCESS;
}

Vector_return vector_pop_no_return(Vector* vector, size_t index)
{
	if(vector == NULL) {return VECTOR_UNITIALIZED_ARGS;}
	if (index < 0 || index >= vector->m_nun_of_items) {return VECTOR_INVALID_INDEX;}

	if(need_to_decrease_capacity(vector))
	{
		Vector_return result = vector_decrease_capacity(vector);
        if(result != VECTOR_SUCCESS)
			return result;
	}

	shrink_array(vector, index);
	vector->m_nun_of_items--;
    return VECTOR_SUCCESS;
}

Vector_return vector_get(const Vector* vector, size_t index, void** return_item)
{
	if(vector == NULL || return_item == NULL) {return VECTOR_UNITIALIZED_ARGS;}
	if (index < 0 || index >= vector->m_nun_of_items) {return VECTOR_INVALID_INDEX;}

	*return_item =  vector->m_array[index];
    return VECTOR_SUCCESS;
}

Vector_return vector_set(Vector* vector, size_t index, void* item)
{
	if(vector == NULL || item == NULL) {return VECTOR_UNITIALIZED_ARGS;}
	if (index < 0 || index >= vector->m_nun_of_items) {return VECTOR_INVALID_INDEX;}

	vector->m_array[index] = item;
    return VECTOR_SUCCESS;
}

size_t vector_capacity (const Vector* vector)
{
	if(vector == NULL)
		return VECTOR_UNITIALIZED_ARGS;

	return vector->m_size;
}

size_t vector_size (const Vector* vector)
{
	if(vector == NULL)
		return VECTOR_UNITIALIZED_ARGS;

	return vector->m_nun_of_items;
}

int vector_empty(const Vector* vector)
{
	if(vector == NULL)
		return VECTOR_UNITIALIZED_ARGS;

	if(vector->m_nun_of_items == 0)
		return TRUE;
	
	return FALSE;
}

void vector_clear(Vector* vector)
{
	if(vector == NULL)
		return;
	
	vector->m_nun_of_items = 0;

	while(need_to_decrease_capacity(vector))
		vector_decrease_capacity(vector);
}

size_t vector_for_each(const Vector* vector, Action action_function, void* context)
{
	if(vector == NULL || action_function == NULL) {return VECTOR_UNITIALIZED_ARGS;}

	size_t i;
	const size_t size = vector->m_nun_of_items;
	for(size_t i = 0; i < size; ++i)
	{
		if(action_function(vector->m_array[i], context) == FALSE)
			break;
	}

	return i;
}