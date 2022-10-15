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

static GroupsManager_return add_client_to_group(Group* group, char* user_name)
{
	Group_return g_result = insert_client_to_group(group, user_name);

	printf("add_client %s to %s group \nnum of client in %s group:%d\n",user_name, group->m_name,  group->m_name, number_of_clients(group));

	if(g_result == GROUP_USER_ALREADY_IN_GROUP){return GROUP_MANAGER_USER_ALREADY_IN_GROUP;}
	if(g_result != GROUP_SUCCESS){return GROUPS_MANAGER_INSERT_CLIENT_FAIL;};
    return GROUPS_MANAGER_SUCCESS;
}

static GroupsManager_return group_already_exists(GroupsManager* groups_manager, const char* group_name, char* user_name, int is_private)
{
	if(is_private)
	{
		Group* group;
		Map_return m_result = hash_map_find(groups_manager->m_groups, (void*)group_name, (void**)&group);
		if (m_result != MAP_SUCCESS) {return GROUPS_MANAGER_NEW_GROUP_FAIL;}
		return add_client_to_group(group, user_name);
	}

	return GROUPS_MANAGER_GROUPNAME_ALREADY_EXISTS;
}

static Group* create_new_group(GroupsManager* groups_manager, const char* group_name, char* return_ip, int is_private)
{
	char* ip;
    queue_remove(groups_manager->m_ips, (void*)&ip);
    Group* group = create_group(group_name, ip, is_private);
    if (group == NULL) {return NULL;}
    strcpy (return_ip, ip);
    free(ip);
	return group;
}

static char* create_key(const char* group_name)
{
    char* key = (char*)malloc(sizeof(group_name));
    if (key == NULL)
        return NULL;

    strcpy(key, group_name);
    return key;
}

static GroupsManager_return insert_group_to_groups_manager_map(GroupsManager* groups_manager, Group* group, const char* group_name)
{
	char* group_name_key = create_key(group_name);
   	if (group_name_key == NULL) {return GROUPS_MANAGER_MALLOC_FAIL;}
   	strcpy (group_name_key, group_name);

    Map_return m_result = hash_map_insert(groups_manager->m_groups, group_name_key, group);
    if (m_result != MAP_SUCCESS)
    {
        free (group_name_key);
        destroy_group (group);
        return GROUPS_MANAGER_NEW_GROUP_FAIL;
    }

	return GROUPS_MANAGER_SUCCESS;
}

GroupsManager_return new_group(GroupsManager* groups_manager, const char* group_name, char* user_name, char* return_ip, int is_private)
{
	if (groups_manager == NULL || group_name == NULL || user_name == NULL || return_ip == NULL)
	    return GROUPS_MANAGER_UNINITIALIZED_ARGS;
	
	if(hash_map_is_exists(groups_manager->m_groups, group_name))
		return group_already_exists(groups_manager, group_name, user_name, is_private);

	Group* group = create_new_group(groups_manager, group_name,  return_ip, is_private);
	if(group == NULL){return GROUPS_MANAGER_CREATE_GROUP_FAIL;}

	GroupsManager_return result = insert_group_to_groups_manager_map(groups_manager,group, group_name);
	if(result != GROUPS_MANAGER_SUCCESS){return result;}

	return add_client_to_group(group, user_name);
}

GroupsManager_return join_existing_group(GroupsManager* groups_manager, char* group_name, char* user_name, char* return_ip)
{
	if (groups_manager == NULL || group_name == NULL || user_name == NULL || return_ip == NULL)
	    return GROUPS_MANAGER_UNINITIALIZED_ARGS;
    
    Group* group;
	Map_return m_result = hash_map_find(groups_manager->m_groups, group_name , (void**)&group);
	if (m_result == MAP_KEY_NOT_EXISTS) {return GROUPS_MANAGER_GROUP_NOT_EXISTS;}
	if(is_private(group)) {return GROUPS_MANAGER_GROUP_NOT_EXISTS;}

	group_ip(group, return_ip);
	return add_client_to_group(group, user_name);
}

static void insert_ip_to_queue(GroupsManager* groups_manager, Group* group)
{
    char* ip = (char*)malloc(sizeof(char)*IP_SIZE);
    group_ip(group, ip);
    queue_insert(groups_manager->m_ips, (void*)ip);
}

GroupsManager_return leave_group(GroupsManager* groups_manager, char* group_name, char* user_name)
{
	if (groups_manager == NULL || group_name == NULL || user_name == NULL )
	    return GROUPS_MANAGER_UNINITIALIZED_ARGS;
	
	printf("%s leave %s group\n", user_name, group_name);

    Group* group;
	Map_return m_result = hash_map_find(groups_manager->m_groups, group_name , (void**)&group);
	if(m_result == MAP_KEY_NOT_EXISTS) {return GROUPS_MANAGER_GROUP_NOT_EXISTS;}
	
	Group_return g_result = remove_cleint_from_group(group, user_name);
	if(g_result == GROUP_USER_NOT_EXISTS){return GROUPS_MANAGER_USER_NOT_EXISTS;}
	if(g_result != GROUP_SUCCESS){return GROUPS_MANAGER_REMOVE_CLIENT_FAIL;};

	printf("num of client in %s group: %d\n", group_name, number_of_clients(group));

	if(number_of_clients(group) < 1)
	{
		printf("group %s deleted\n", group_name);
        insert_ip_to_queue(groups_manager, group);
		hash_map_remove_and_free(groups_manager->m_groups, group_name);
		return GROUPS_MANAGER_GROUP_DELETED;
	}

	return GROUPS_MANAGER_SUCCESS;
}

GroupsManager_return leave_all_groups(GroupsManager* groups_manager, List* list_of_user_groups, char* user_name)
{
	if (groups_manager == NULL || list_of_user_groups == NULL || user_name == NULL)
	    return GROUPS_MANAGER_UNINITIALIZED_ARGS;

	ListItr it_next;
	ListItr it = begin(list_of_user_groups);
	ListItr it_end = end(list_of_user_groups);
	while(it != it_end)
	{
		it_next = next(it);
		leave_group(groups_manager, ((Group*)get_data(it))->m_name, user_name);
		it = it_next;
	}

	return GROUPS_MANAGER_SUCCESS;
}

static int check_if_private(char* name)
{
	char check[STRING_SIZE] = {0};
	strncpy(check, name, 8);
	if(!strcmp(check, "\"Private"))
		return TRUE;
	return FALSE;
}

static void write_key_to_buffer(void* name, char* names_list)
{
	if(check_if_private((char*)name))
		return;

	strcat(names_list, " * ");
    strcat(names_list, (char*)name);
	strcat(names_list, "\n");
}

void give_all_groups_names(GroupsManager* groups_manager, char* groups_names_list)
{
    if (groups_manager == NULL || groups_names_list == NULL) {return;}

    strcat(groups_names_list, "\nExisting groups:\n");
    give_all_keys_names(groups_manager->m_groups, groups_names_list, write_key_to_buffer);
}

static void sprint_group_name(void* hash_element, void* users_names_list)
{
	char open_message[OPEN_MESSAGE_SIZE];
	sprintf(open_message, "\nIn %s group:\n", ((Group*)((Element*)hash_element)->m_value)->m_name);
	strcat(users_names_list, open_message);
}

static int give_users_names(void* hash_element, void* users_names_list)
{
	if(((Group*)((Element*)hash_element)->m_value)->m_is_private)
		return TRUE;

	sprint_group_name(hash_element, users_names_list);
	give_all_keys_names(((Group*)((Element*)hash_element)->m_value)->m_users, (char*)users_names_list, write_key_to_buffer);
	return TRUE;
}

void give_all_users_names(GroupsManager* groups_manager, char* users_names_list)
{
    if (groups_manager == NULL || users_names_list == NULL) {return;}

	strcat(users_names_list, "\nExisting users:\n");
	hash_map_for_each(groups_manager->m_groups, give_users_names , users_names_list);
}

int num_of_groups(GroupsManager* groups_manager)
{
	if(groups_manager == NULL) {return 0;}
	return hash_map_size(groups_manager->m_groups);
}

int is_a_private(GroupsManager* groups_manager, char* group_name)
{
	if (groups_manager == NULL || group_name == NULL) {return FALSE;}

	Group* group;
	Map_return m_result = hash_map_find(groups_manager->m_groups, group_name , (void**)&group);
	if(m_result == MAP_KEY_NOT_EXISTS) {return FALSE;}
	return is_private(group);
}

HashMap* group_users(GroupsManager* groups_manager, char* group_name)
{
	if (groups_manager == NULL || group_name == NULL) {return FALSE;}

	Group* group;
	Map_return m_result = hash_map_find(groups_manager->m_groups, group_name , (void**)&group);
	if(m_result == MAP_KEY_NOT_EXISTS) {return FALSE;}
	return give_users_list(group);
}

