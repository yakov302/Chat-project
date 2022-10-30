#ifndef GROUP_H
#define GROUP_H

#include <stdlib.h>
#include <string.h>

#include "hash_map.h"

#define MAGIC_NUMBER 144522
#define STRING_SIZE 68
#define NOT_EQUAL 0
#define EQUAL 1

typedef enum Group_return
{
	GROUP_SUCCESS,
	GROUP_MALLOC_FAIL,
    GROUP_ARGS_NOT_INITIALIZED,
	GROUP_INSERT_MAP_FAIL,
	GROUP_REMOVE_FROM_MAP_FAIL,
	GROUP_USER_ALREADY_IN_GROUP,
	GROUP_USER_NOT_EXISTS

}Group_return;

typedef struct Group
{
	char m_name[STRING_SIZE];
	char m_ip[STRING_SIZE];
	int m_num_of_clients;
	int m_magic_number;
	int m_is_private;
	HashMap* m_users;

}Group;

Group* create_group(const char* group_name, char* group_ip, int is_private);

void destroy_group(Group* group);

void group_ip(Group* group, char* group_ip);

int number_of_clients(Group* group);

Group_return insert_client_to_group(Group* group, char* user_name);

Group_return remove_cleint_from_group(Group* group, char* user_name);

int is_private(Group* group);

HashMap* give_users_list(Group* group);


#endif //GROUP_H

