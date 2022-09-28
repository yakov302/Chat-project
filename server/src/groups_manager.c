#include "groups_manager.h"

static size_t hash_for_group_name(void* group_name)
{
	size_t mul = 1;
	const int len = strlen ((char*)group_name);
	
	for (int i = 0; i < len; ++i)
		mul *= ((char*)group_name)[i]+((i+1));

	return mul;
}

static int compare_group_names (const void* hash_element, const void* group_name)	
{
	if (strcmp ((char*)((Group*)((Element*)hash_element)->m_value)->m_name, (char*)group_name) == 0)
	{
		return EQUAL;
	}
	return NOT_EQUAL;
}

static void destroy_group_element(void* element) 
{
    if(element != NULL)
    {
	    destroy_group(((Element*)element)->m_value);
        free(((Element*)element)->m_key);
        free(element);
    }
}

static void destroy_ip(void* ip) 
{
    if(ip != NULL)
    {
        free(ip);
        ip = NULL;
    }
}

static int print_ips(void* element, void* context)
{
    printf("%s\n", (char*)element);
}

static void fill_queue_with_ips(Queue* queue)
{
	char ip_format [IP_SIZE] = "224.255.255.";
	char number[IP_SIZE];

	for(int i = 1; i < NUM_OF_IPS; ++i)
	{
        char* ip = (char*)malloc(sizeof(char)*IP_SIZE);
		if(ip == NULL)
		    return;

		sprintf(number, "%d", i);
		strcpy(ip, ip_format);
		strcat(ip, number);
		queue_insert(queue, ip);	
	}
    //queue_for_each(queue, print_queue, NULL);
}

GroupsManager* create_groups_manager(int capacity)
{
	GroupsManager* groups_manager = (GroupsManager*)malloc(sizeof(GroupsManager));
	if (groups_manager == NULL)
	    return NULL;
	
	groups_manager->m_groups = hash_map_create(capacity, hash_for_group_name, compare_group_names, destroy_group_element);
	if (groups_manager->m_groups == NULL)
	{
		free(groups_manager);
		return NULL; 
	}
		
	groups_manager->m_ips = queue_create(NUM_OF_IPS, destroy_ip);
	if (groups_manager->m_ips == NULL)
	{
		hash_map_destroy(&groups_manager->m_groups);
		free(groups_manager);
		return NULL; 
	}

    groups_manager->m_magic_number = MAGIC_NUMBER;
	fill_queue_with_ips(groups_manager->m_ips);
	return groups_manager;
}

void destroy_groups_manager(GroupsManager* groups_manager)
{
	if (groups_manager == NULL || groups_manager->m_magic_number != MAGIC_NUMBER)
	    return;
	
	groups_manager->m_magic_number = 0;
    queue_destroy(&groups_manager->m_ips);
	hash_map_destroy(&groups_manager->m_groups);
	free(groups_manager);
}

static char* create_key(const char* group_name)
{
    char* key = (char*)malloc(sizeof(group_name));
    if (key == NULL)
        return NULL;

    strcpy(key, group_name);
    return key;
}

GroupsManager_return new_group(GroupsManager* groups_manager, const char* group_name, char* return_ip)
{
	if (groups_manager == NULL || group_name == NULL || return_ip == NULL)
	    return GROUPS_MANAGER_UNINITIALIZED_ARGS;
	
	if(hash_map_is_exists(groups_manager->m_groups, group_name))
        return GROUPS_MANAGER_GROUPNAME_ALREADY_EXISTS;

    char* ip;
    queue_remove(groups_manager->m_ips, (void*)&ip);
    Group* group = create_group(group_name, ip);
    if (group == NULL) {return GROUPS_MANAGER_CREATE_GROUP_FAIL;}
    strcpy (return_ip, ip);
    free(ip);

    char* group_name_key = create_key(group_name);
   if (group_name_key == NULL) {return GROUPS_MANAGER_MALLOC_FAIL;}
   strcpy (group_name_key, group_name);

    Map_return result = hash_map_insert(groups_manager->m_groups, group_name_key, group);
    if (result != MAP_SUCCESS)
    {
        free (group_name_key);
        destroy_group (group);
        return GROUPS_MANAGER_NEW_GROUP_FAIL;
    }
    
    increse_num_of_cleints(group);
    return GROUPS_MANAGER_SUCCESS;
}

GroupsManager_return join_existing_group(GroupsManager* groups_manager, char* group_name, char* return_ip)
{
	if (groups_manager == NULL || group_name == NULL || return_ip == NULL)
	    return GROUPS_MANAGER_UNINITIALIZED_ARGS;
    
    Group* group;
	Map_return result = hash_map_find(groups_manager->m_groups, group_name , (void**)&group);
	if (result == MAP_KEY_NOT_EXISTS) {return GROUPS_MANAGER_GROUP_NOT_EXISTS;}

	group_ip(group, return_ip);
	increse_num_of_cleints(group);
	return GROUPS_MANAGER_SUCCESS;
}

static void insert_ip_to_queue(GroupsManager* groups_manager, Group* group)
{
    char* ip = (char*)malloc(sizeof(char)*IP_SIZE);
    group_ip(group, ip);
    queue_insert(groups_manager->m_ips, (void*)ip);
}

GroupsManager_return leave_group (GroupsManager* groups_manager, char* group_name)
{
	if (groups_manager == NULL || group_name == NULL)
	    return GROUPS_MANAGER_UNINITIALIZED_ARGS;
	
    Group* group;
	Map_return result = hash_map_find(groups_manager->m_groups, group_name , (void**)&group);
	if(result == MAP_KEY_NOT_EXISTS) {return GROUPS_MANAGER_GROUP_NOT_EXISTS;}
	
	decrese_num_of_cleints (group);
	if(number_of_clients(group) < 1)
	{
        insert_ip_to_queue(groups_manager, group);
		hash_map_remove_and_free(groups_manager->m_groups, group_name);
		return GROUPS_MANAGER_GROUP_DELETED;
	}

	return GROUPS_MANAGER_SUCCESS;
}

static void write_key_to_buffer(void* group_name, char* groups_names_list)
{
    strcat(groups_names_list, (char*)group_name);
	strcat(groups_names_list, "\n");
}

void give_all_groups_names(GroupsManager* groups_manager, char* groups_names_list)
{
    if (groups_manager == NULL || groups_names_list == NULL) {return;}
    
    give_all_keys_names(groups_manager->m_groups, groups_names_list, write_key_to_buffer);
}

int num_of_groups(GroupsManager* groups_manager)
{
	if(groups_manager == NULL) {return 0;}
	return hash_map_size(groups_manager->m_groups);
}

