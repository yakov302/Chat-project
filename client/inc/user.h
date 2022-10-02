#ifndef USER_H
#define USER_H

#include <string.h>

#include "list_iterator.h"
#include "group.h"


#define MAGIC_NUMBER 144522
#define STRING_SIZE 68

typedef enum User_return
{
	USER_SUCCESS,
	USER_MALLOC_FAIL,
    USER_ARGS_NOT_INITIALIZED,
	USER_NOT_INITIALIZED,
	USER_GROUP_NOT_EXISTS,
	USER_ALREADY_IN_THE_GROUP

}User_return;

typedef struct User
{
	char m_name[STRING_SIZE];
    List* m_groups;
    int m_is_logged_in;
	int m_magic_number;

}User;

User* create_user(const char* name);

void destroy_user(User* user);

int is_logged_in(User* user);

void set_is_logged_in(User* user, int status);

User_return set_name(User* user, char* name);

User_return add_group_for_user(User* user, Group* group);

User_return remove_group_from_user(User* user, Group* group);

Group* group(User* user, char* group_name);


#endif // USER_H