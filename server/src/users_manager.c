#include "users_manager.h"

static size_t hash_for_user_name(void* name)
{
	size_t mul = 1;
	const int len = strlen ((char*)name);

	for (int i = 0; i < len; ++i)
		mul *= ((char*)name)[i]+((i+1));

	return mul;
}

static int compare_user_names (const void* hash_element, const void* user_name)	
{
	if (strcmp ((char*)((User*)((Element*)hash_element)->m_value)->m_name, (char*)user_name) == 0)
	{
		return EQUAL;
	}
	return NOT_EQUAL;
}

static void destroy_user_element(void* element) 
{
    if(element != NULL)
    {
	    destroy_user(((Element*)element)->m_value);
        free(((Element*)element)->m_key);
        free(element);
    }
}

UsersManager* create_users_manager(int capacity)
{
    if(capacity < 1)
        return NULL;

	UsersManager* users_manager = (UsersManager*)malloc(sizeof(UsersManager));
	if (users_manager == NULL)
	    return NULL;

	users_manager->m_users = hash_map_create(capacity, hash_for_user_name, compare_user_names, destroy_user_element);
	if (users_manager->m_users == NULL)
	{
		free(users_manager);
		return NULL; 
	}

    users_manager->m_magic_number = MAGIC_NUMBER;
	return users_manager;
}

void destroy_users_manager(UsersManager* user_manager)
{
	if (user_manager == NULL || user_manager->m_magic_number != MAGIC_NUMBER)
	    return;

	user_manager->m_magic_number = 0;
	hash_map_destroy(&user_manager->m_users); 
	free (user_manager);
    user_manager = NULL;
}

static char* create_key(const char* name)
{
    char* user_name = (char*)malloc(sizeof(name));
    if (user_name == NULL)
        return NULL;

    strcpy(user_name, name);
    return user_name;
}

UsersManager_return user_log_in(UsersManager* users_manager, const char* name, int socket)
{
	if (users_manager == NULL || name == NULL)
		return USER_MANAGER_ARGS_NOT_INITIALIZED;

	if (hash_map_is_exists(users_manager->m_users, name)) {return USER_MANAGER_USER_ALREADY_LOGGED_IN;}

    char* user_name = create_key(name);
    if (user_name == NULL) {return USER_MANAGER_MALLOC_FAIL;}

    User* user = create_user(name, socket);
    if (user == NULL)
    {   
        free(user_name);
        return USER_MANAGER_MALLOC_FAIL;
    }

    Map_return status = hash_map_insert(users_manager->m_users, user_name, user);
    if (status != MAP_SUCCESS)
    {
        free (user_name);
        destroy_user (user);
        return USER_MANAGER_LOG_IN_FAIL;
    }
            
    return USER_MANAGER_SUCCESS;
}

UsersManager_return user_join_group (UsersManager* users_manager, char* user_name, char* group_name)
{
	if (users_manager == NULL || user_name == NULL || group_name == NULL)
	    return USER_MANAGER_ARGS_NOT_INITIALIZED;

    void* user;
	Map_return m_result = hash_map_find(users_manager->m_users, user_name, &user);
	if (m_result != MAP_SUCCESS) {return USER_MANAGER_USER_NOT_EXISTS;}

    User_return u_result = add_group_for_user((User*)user, group_name);
	if (u_result == USER_ALREADY_IN_THE_GROUP) {return USER_MANAGER_USER_ALREADY_IN_THE_GROUP;}
    else if(u_result == USER_SUCCESS) {return USER_MANAGER_SUCCESS;}
	return USER_MANAGER_JOIN_GROUP_FAIL;
}
	
UsersManager_return user_leave_group (UsersManager* users_manager, char* user_name, char* group_name)
{
	if (users_manager == NULL || user_name == NULL || group_name == NULL)
	    return USER_MANAGER_ARGS_NOT_INITIALIZED;

    void* user;
	Map_return m_result = hash_map_find(users_manager->m_users, user_name, &user);
	if (m_result != MAP_SUCCESS) {return USER_MANAGER_USER_NOT_EXISTS;}

	User_return u_result = remove_group_from_user((User*)user, group_name); 
    if (u_result == USER_GROUP_NOT_EXISTS) {return USER_MANAGER_GROUP_NOT_EXISTS;}
    else if(u_result == USER_SUCCESS) {return USER_MANAGER_SUCCESS;}
	return USER_MANAGER_LEAVE_GROUP_FAIL;
}

List* user_groups_list(UsersManager* users_manager, char* user_name)
{
	if (users_manager == NULL || user_name == NULL)
	    return NULL;
		
    void* user;
	Map_return m_result = hash_map_find(users_manager->m_users, user_name, &user);
	if (m_result != MAP_SUCCESS) {return USER_MANAGER_USER_NOT_EXISTS;}
	return ((User*)user)->m_groups;
}

UsersManager_return user_log_out(UsersManager* users_manager, char* user_name)
{
	if (users_manager == NULL || user_name == NULL)
	    return USER_MANAGER_ARGS_NOT_INITIALIZED;
		
	if (!hash_map_is_exists(users_manager->m_users, user_name)) {return USER_MANAGER_USER_NOT_EXISTS;}
	Map_return result = hash_map_remove_and_free(users_manager->m_users , user_name);
    if(result != MAP_SUCCESS) {return USER_MANAGER_LOGE_OUT_FAIL;}
	return USER_MANAGER_SUCCESS;
}

static int find_user_by_socket(const void* hash_element, const void* socket)
{
    if(((User*)((Element*)hash_element)->m_value)->m_socket == *(int*)socket)
        return TRUE;
    return FALSE;
}

UsersManager_return user_disconnected(UsersManager* users_manager, int socket)
{
	if (users_manager == NULL)
	    return USER_MANAGER_ARGS_NOT_INITIALIZED;
	
	Element* element = hash_map_find_by_customize_key(users_manager->m_users, find_user_by_socket, &socket);
	if(element == NULL) {return USER_MANAGER_USER_NOT_EXISTS;}
	user_log_out(users_manager, ((User*)(element->m_value))->m_name);
}

	