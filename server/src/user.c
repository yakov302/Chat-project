#include "user.h"

User* create_user(const char* name, int socket)
{
	if(name == NULL)
		return NULL;

	User* user = (User*)malloc(sizeof(User));
	if (user == NULL)
	    return NULL;

	user->m_groups = list_create();
	if (user->m_groups == NULL)
	{
		free(user);
		return NULL;
	}

	strcpy (user->m_name, name);
	user->m_socket = socket;
    user->m_magic_number = MAGIC_NUMBER;
	return user;
}

static void destroy_grup_name(void* name)
{
	free(name);
}

void destroy_user(User* user)
{
	if (user == NULL || user->m_magic_number != MAGIC_NUMBER)
	    return;

	user->m_magic_number = 0;
	list_destroy(&user->m_groups, destroy_grup_name);
	free (user);
    user == NULL;
}

static int is_group_name_equal(const void* a, const void* b)
{
    if(strcmp((char*)a, (char*)b) == 0)
        return YES;
    return NO;
}

User_return add_group_for_user(User* user, char* group_name)
{
    if (user == NULL || group_name == NULL)
	    return USER_ARGS_NOT_INITIALIZED;

    if(list_is_exists(user->m_groups, is_group_name_equal, group_name))
        return USER_ALREADY_IN_THE_GROUP;

	char* name = malloc(sizeof(char)*STRING_SIZE);
	if (name == NULL)
	    return USER_ARGS_NOT_INITIALIZED;

	strcpy (name, group_name);
	list_push_tail(user->m_groups, (void*)name);
	return USER_SUCCESS;
}
	
User_return remove_group_from_user(User* user, char* group_name)
{
    if (user == NULL || group_name == NULL)
	    return USER_ARGS_NOT_INITIALIZED;

    ListItr it = find_first(user->m_groups, is_group_name_equal, group_name);
    if(it != NULL)
    {
        free(remove_it(user->m_groups, it));
		return USER_SUCCESS;
    }
    
	return USER_GROUP_NOT_EXISTS;
}

