#include "../inc/group.h"

static size_t hash_for_user_name(void* user_name)
{
	size_t mul = 1;
	const int len = strlen ((char*)user_name);
	
	for (int i = 0; i < len; ++i)
		mul *= ((char*)user_name)[i]+((i+1));

	return mul;
}

static int compare_user_names(const void* hash_element, const void* user_name)	
{
	if (strcmp ((char*)(((Element*)hash_element)->m_key), (char*)user_name) == 0)
	{
		return EQUAL;
	}
	return NOT_EQUAL;
}

static void destroy_user_element(void* element) 
{
    if(element != NULL)
    {
        free(((Element*)element)->m_key);
        free(element);
    }
}

Group* create_group(const char* group_name, char* group_ip, int is_private)
{
    Group* group = (Group*)malloc(sizeof(Group));
	if(group == NULL)
	    return NULL;

	group->m_users = hash_map_create(200, hash_for_user_name, compare_user_names, destroy_user_element);
	if(group->m_users == NULL)
	{
		free(group);
		return NULL;
	}

	strcpy (group->m_name, group_name);
    strcpy (group->m_ip, group_ip);
	group->m_num_of_clients = 0;
	group->m_is_private = is_private;
	group->m_magic_number = MAGIC_NUMBER;
	return group;
}

void destroy_group(Group* group)
{
    if (group == NULL || group->m_magic_number != MAGIC_NUMBER)
	    return;
	
	group->m_magic_number = 0;
	hash_map_destroy(&group->m_users);
	free (group);
    group = NULL;
}

void group_ip(Group* group, char* group_ip)
{
    if(group == NULL || group_ip == NULL)
	    return;
	
	strcpy(group_ip, group->m_ip);
}

int number_of_clients(Group* group)
{
    if(group == NULL)
	    return 0;
	
    return group->m_num_of_clients;
}

static char* create_key(char* name)
{
    char* user_name = (char*)malloc(sizeof(char)*strlen(name));
    if (user_name == NULL)
        return NULL;

    strcpy(user_name, name);
    return user_name;
}

Group_return insert_client_to_group(Group* group, char* user_name)
{

    if(group == NULL || user_name == NULL)
	    return GROUP_ARGS_NOT_INITIALIZED;

	char* name = create_key(user_name);
    if (name == NULL) {return GROUP_MALLOC_FAIL;}

	Map_return result = hash_map_insert(group->m_users, name, user_name);
	if(result == MAP_KEY_ALREADY_EXISTS) {return GROUP_USER_ALREADY_IN_GROUP;}
	if(result != MAP_SUCCESS){return GROUP_INSERT_MAP_FAIL;}

    group->m_num_of_clients++; 
	return GROUP_SUCCESS;
}

Group_return remove_cleint_from_group (Group* group, char* user_name)
{
    if(group == NULL || user_name == NULL)
	    return GROUP_ARGS_NOT_INITIALIZED;
	
	Map_return result = hash_map_remove_and_free(group->m_users, user_name);
	if(result == MAP_KEY_NOT_EXISTS){return GROUP_USER_NOT_EXISTS;}
	if(result != MAP_SUCCESS){return GROUP_REMOVE_FROM_MAP_FAIL;}

    group->m_num_of_clients--; 
	return GROUP_SUCCESS;
}

int is_private(Group* group)
{
	if(group == NULL)
	    return FALSE;
	
	return group->m_is_private;
}

HashMap* give_users_list(Group* group)
{
	if(group == NULL)
	    return FALSE;
	
	return group->m_users;
}
