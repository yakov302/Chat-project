#include "hash_map.h"

static int is_prime(int number) 
{
  if (number == 0 || number == 1) 
    return FALSE;
    
  for (int i = 2; i < number; ++i) 
  {
    if (number%i == 0) 
        return FALSE;
  }

  return TRUE;
}

static size_t calculate_capacity(size_t capacity)
{
	while(!is_prime(capacity))
		capacity++;
		
    return capacity;
}

HashMap* hash_map_create(size_t capacity, HashFunction hash_function, ComparisonFunction comparison_function, ElementDestroy element_destroy)
{
	if(capacity == 0 || hash_function == NULL || comparison_function == NULL)
		return NULL;

    HashMap* hash_map = (HashMap*)malloc(sizeof(HashMap));
	if(hash_map == NULL)
        return NULL;

   	size_t finel_capacity = calculate_capacity(capacity); 
    hash_map->m_lists = (List**)calloc(finel_capacity , sizeof(List*));
	if (hash_map->m_lists == NULL)
	{
		free(hash_map);
		return NULL;
	}

	hash_map->m_hash_function = hash_function;
	hash_map->m_comparison_function = comparison_function;
    hash_map->m_element_destroy = element_destroy;
	hash_map->m_capacity = finel_capacity;
	hash_map->m_num_of_item = 0;
    return hash_map;
}

void hash_map_destroy(HashMap** map)
{
	if(map == NULL || *map == NULL){return;}
	
    size_t i;
	for(i = 0; i < (*map)->m_capacity; ++i)
	{
		if((*map)->m_lists[i] != NULL)
			list_destroy(&(*map)->m_lists[i], (*map)->m_element_destroy);		
	}
	
	free((*map)->m_lists);
	free(*map);
	*map = NULL;
}

static void* create_element(const void* key, const void* value)
{
    void* element  = (Element*)malloc(sizeof(Element));
	if(element == NULL)
        return NULL;
	
	((Element*)element)->m_key = (void*)key;
	((Element*)element)->m_value = (void*)value;
    return element;
}

static Map_return push_element(List* list, const void* key, const void* value)
{
    Element* element = create_element(key, value);
    if(element == NULL)
        return MAP_ELEMENT_ALLOCATION_FAIL;

    if(list_push_head(list, element) != LIST_SUCCESS)
        return MAP_LIST_INSERT_FAIL;

    return MAP_SUCCESS;
}

Map_return hash_map_insert(HashMap* map, const void* key, const void* value)
{
	if(map == NULL || key == NULL || value == NULL){return MAP_UNINITIALIZED_ARGS;}

	size_t index = map->m_hash_function((void*)key)%map->m_capacity;
	if(map->m_lists[index] == NULL)
	{
		if((map->m_lists[index] = list_create()) == NULL)
			return MAP_LIST_ALLOCATION_FAIL;

		int result = push_element(map->m_lists[index], key, value);
        if(result != MAP_SUCCESS)
            return result;		
	}
    else
    {
        if(list_is_exists(map->m_lists[index], map->m_comparison_function, key) == YES)
            return MAP_KEY_ALREADY_EXISTS;

		int result = push_element(map->m_lists[index], key, value);
        if(result != MAP_SUCCESS)
            return result;
    }

    map->m_num_of_item++;
    return MAP_SUCCESS;
}

Map_return hash_map_remove_and_free(HashMap* map, const void* key)
{
 	if(map == NULL || key == NULL) {return MAP_UNINITIALIZED_ARGS;}

	size_t index = map->m_hash_function((void*)key)%map->m_capacity;
	if(map->m_lists[index] == NULL){return MAP_KEY_NOT_EXISTS;}
		
	ListItr it = find_first(map->m_lists[index], map->m_comparison_function, key);
	if(it == NULL){return MAP_KEY_NOT_EXISTS;}
		
	Element* element = remove_it(map->m_lists[index], it);
    map->m_element_destroy(element);
	map->m_num_of_item--;
    return MAP_SUCCESS;   
}

Map_return hash_map_remove(HashMap* map, const void* key, void** key_ptr, void** value_ptr)
{
	if(map == NULL || key == NULL || key_ptr == NULL || value_ptr == NULL)
        return MAP_UNINITIALIZED_ARGS;

	size_t index = map->m_hash_function((void*)key)%map->m_capacity;
	if(map->m_lists[index] == NULL){return MAP_KEY_NOT_EXISTS;}
		
	ListItr it = find_first(map->m_lists[index], map->m_comparison_function, key);
	if(it == NULL){return MAP_KEY_NOT_EXISTS;}
		
	Element* element = remove_it(map->m_lists[index], it);
	*key_ptr = ((Element*)element)->m_key;
	*value_ptr = ((Element*)element)->m_value;

	free(element);	
	map->m_num_of_item--;
    return MAP_SUCCESS;
}

int hash_map_is_exists(const HashMap* map, const void* key)
{
    if(map == NULL || key == NULL)
        return FALSE;

    size_t index = map->m_hash_function((void*)key)%map->m_capacity;
	if(map->m_lists[index] == NULL){return FALSE;}
	return list_is_exists(map->m_lists[index], map->m_comparison_function, key);
}

Map_return hash_map_find(const HashMap* map, const void* key, void** value_ptr)
{
	if(map == NULL || key == NULL || value_ptr == NULL)
        return MAP_UNINITIALIZED_ARGS;
	
    size_t index = map->m_hash_function((void*)key)%map->m_capacity;
	if(map->m_lists[index] == NULL){return MAP_KEY_NOT_EXISTS;}
	
	ListItr it = find_first(map->m_lists[index], map->m_comparison_function, key);
	if(it == NULL){return MAP_KEY_NOT_EXISTS;}
	
	*value_ptr = ((Element*)get_data(it))->m_value;
    return MAP_SUCCESS;
}

Element* hash_map_find_by_customize_key(const HashMap* map, ComparisonFunction customize_compar , void* customize_key)
{
	if(map == NULL || customize_compar == NULL || customize_key == NULL)
        return NULL;
	
	for(int i = 0; i < map->m_capacity; ++i)
	{
		if(map->m_lists[i] != NULL)
		{
			Element* it = find_first(map->m_lists[i],customize_compar, customize_key);
			if(it != NULL)
				return (Element*)get_data(it);
		}
	}
	
    return NULL;
}

size_t hash_map_size(const HashMap* map)
{
    if(map == NULL){return MAP_UNINITIALIZED_ARGS;}
    return map->m_num_of_item;
}

void hash_map_print(HashMap* map, PrintItem print_element)
{
	if(map == NULL || print_element == NULL) {return;}

	for(int i = 0; i < map->m_capacity; ++i)
	{
		if(map->m_lists[i] != NULL)
			list_print(map->m_lists[i], print_element);		
	}
}

static void write_keys_name(List* list, char* key_list, WriteKeyToBuffer write_key_to_buffer)
{
	ListItr it  = begin(list);
	ListItr end_it  = end(list);

	while(it != end_it)
	{
		Element* element = (Element*)get_data(it);
		write_key_to_buffer(element->m_key, key_list);
		it = next(it);
	}
}

void give_all_keys_names(HashMap* map, char* key_list, WriteKeyToBuffer write_key_to_buffer)
{
	if(map == NULL || key_list == NULL || write_key_to_buffer == NULL) 
		return;

	for(int i = 0; i < map->m_capacity; ++i)
	{
		if(map->m_lists[i] != NULL)
			write_keys_name(map->m_lists[i], key_list, write_key_to_buffer);		
	}
}

