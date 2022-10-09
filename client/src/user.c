#include "user.h"

User* create_user(const char* name)
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
    user->m_is_logged_in = FALSE;
    user->m_magic_number = MAGIC_NUMBER;
	return user;
}

static void destroy_grup(void* group)
{
	group_destroy((Group*)group);
}

void destroy_user(User* user)
{
	if (user == NULL || user->m_magic_number != MAGIC_NUMBER)
	    return;

	user->m_magic_number = 0;
	list_destroy(&user->m_groups, destroy_grup);
	free (user);
    user == NULL;
}

int is_logged_in(User* user)
{
    if(user == NULL)
    {
        printf("is_logged_in user not initialized!\n");
        return FALSE;
    }

    return user->m_is_logged_in;
}

void set_log_in_status(User* user, int status)
{
    if(user == NULL)
    {
        printf("set_log_in_status user not initialized!\n");
        return;
    }

    user->m_is_logged_in = status;
}


User_return set_name(User* user, char* name)
{
    if (user == NULL || name == NULL)
	    return USER_ARGS_NOT_INITIALIZED;
    
    strcpy(user->m_name, name);
    return USER_SUCCESS;
}

static int is_groups_equal(const void* a, const void* b)
{
    if(strcmp((char*)((Group*)a)->m_name, (char*)((Group*)b)->m_name) == 0)
        return YES;
    return NO;
}

User_return add_group_for_user(User* user, Group* group)
{
    if (user == NULL || group == NULL)
	    return USER_ARGS_NOT_INITIALIZED;

    if(list_is_exists(user->m_groups, is_groups_equal, group))
        return USER_ALREADY_IN_THE_GROUP;

	list_push_tail(user->m_groups, group);
	return USER_SUCCESS;
}
	
User_return remove_group_from_user(User* user, Group* group)
{
    if (user == NULL || group == NULL)
	    return USER_ARGS_NOT_INITIALIZED;

    ListItr it = find_first(user->m_groups, is_groups_equal, group);
    if(it != NULL)
    {
        remove_it(user->m_groups, it);
		return USER_SUCCESS;
    }
    
	return USER_GROUP_NOT_EXISTS;
}

static int remove_group(void* group, void* user)
{
    if((Group*)group == NULL)
        return TRUE; 

    remove_group_from_user((User*)user, (Group*)group);
    group_destroy((Group*)group);
    return TRUE;
}

User_return remove_all_groups_from_user(User* user)
{
    if (user == NULL)
	    return USER_ARGS_NOT_INITIALIZED;

    list_for_each(user->m_groups, remove_group, user);
	return USER_SUCCESS;
}

static int is_groups_equal_by_name(const void* a, const void* b)
{
    if(strcmp((char*)((Group*)a)->m_name, (char*)b) == 0)
        return YES;
    return NO;
}

Group* group(User* user, char* group_name)
{
    if(group_name == NULL || group_name == NULL)
        return NULL;

    ListItr it = find_first(user->m_groups, is_groups_equal_by_name, group_name);
    if(it != NULL)
		return get_data(it);

    return NULL;
}

List* groups_list(User* user)
{
    if (user == NULL)
	    return NULL;
    
    return user->m_groups;
}

char* name(User* user)
{
    if(user == NULL)
        return NULL;

    return user->m_name;
}
