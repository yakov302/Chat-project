#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "list_iterator.h"

#define TRUE 1
#define FALSE 0

typedef struct Element
{
    void* m_key;
    void* m_value;

}Element;

typedef int(*ActionFunction)(void* element, void* context);
typedef size_t (*HashFunction)(void* key);
typedef int (*ComparisonFunction)(const void* hash_element, const void* key);
typedef void (*ElementDestroy)(void* element);
typedef void (*WriteKeyToBuffer)(void* key, char* key_list);


typedef struct HashMap
{
    List** m_lists;                
    HashFunction m_hash_function;
    ComparisonFunction m_comparison_function;
    ElementDestroy m_element_destroy;
    size_t m_capacity;
    size_t m_num_of_item;

}HashMap;

typedef enum Map_return {
	MAP_SUCCESS,
	MAP_UNINITIALIZED_ARGS, 					
	MAP_KEY_ALREADY_EXISTS, 		
	MAP_KEY_NOT_EXISTS, 		
	MAP_ALLOCATION_FAIL, 
	MAP_LIST_ALLOCATION_FAIL,
	MAP_ELEMENT_ALLOCATION_FAIL,
	MAP_LIST_INSERT_FAIL	

} Map_return;


HashMap* hash_map_create(size_t capacity, HashFunction hash_function, ComparisonFunction comparison_function, ElementDestroy element_destroy);

void hash_map_destroy(HashMap** map);

Map_return hash_map_insert(HashMap* map, const void* key, const void* value);

Map_return hash_map_remove_and_free(HashMap* map, const void* key);

Map_return hash_map_remove(HashMap* map, const void* key, void** key_ptr, void** value_ptr);

int hash_map_is_exists(const HashMap* map, const void* key);

Map_return hash_map_find(const HashMap* map, const void* key, void** value_ptr);

Element* hash_map_find_by_customize_key(const HashMap* map, ComparisonFunction customize_compar , void* customize_key); // o(n)

size_t hash_map_size(const HashMap* map);

void hash_map_print(HashMap* map, PrintItem print_element);

void give_all_keys_names(HashMap* map, char* key_list, WriteKeyToBuffer write_key_to_buffer);

Element* hash_map_for_each(HashMap* map, ActionFunction action , void* context);


#endif // HASH_MAP_H