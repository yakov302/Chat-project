#ifndef GROUPS_MANAGER_H
#define GROUPS_MANAGER_H

#include <stdlib.h>
#include <string.h>

#include "group.h"
#include "queue.h"
#include "hash_map.h"

#define EQUAL 1
#define IP_SIZE 20
#define NOT_EQUAL 0
#define NUM_OF_IPS 250
#define OPEN_MESSAGE_SIZE 100
#define MAGIC_NUMBER 144522

typedef struct GroupsManager
{
	HashMap* m_groups;
	Queue* m_ips;
	int m_magic_number;

}GroupsManager;

typedef enum GroupsManager_return
{
    GROUPS_MANAGER_UNINITIALIZED_ARGS,
    GROUPS_MANAGER_GROUPNAME_ALREADY_EXISTS,
    GROUPS_MANAGER_GROUP_NOT_EXISTS,
    GROUPS_MANAGER_MALLOC_FAIL,
    GROUPS_MANAGER_CREATE_GROUP_FAIL,
    GROUPS_MANAGER_NEW_GROUP_FAIL,
    GROUPS_MANAGER_SUCCESS,
    GROUPS_MANAGER_GROUP_DELETED,
    GROUPS_MANAGER_INSERT_CLIENT_FAIL,
    GROUPS_MANAGER_REMOVE_CLIENT_FAIL,
    GROUP_MANAGER_USER_ALREADY_IN_GROUP,
    GROUPS_MANAGER_USER_NOT_EXISTS

}GroupsManager_return;


GroupsManager* create_groups_manager(int capacity);

void destroy_groups_manager(GroupsManager* groups_manager);

GroupsManager_return new_group(GroupsManager* groups_manager, char* group_name, char* user_name, char* return_ip, int is_private);

GroupsManager_return join_existing_group (GroupsManager* groups_manager, char* group_name, char* user_name, char* return_ip);

GroupsManager_return leave_group (GroupsManager* groups_manager, char* group_name, char* user_name);

GroupsManager_return leave_all_groups(GroupsManager* groups_manager, List* list_of_user_groups, char* user_name);

void give_all_groups_names(GroupsManager* groups_manager, char* groups_names_list);

void give_all_users_names(GroupsManager* groups_manager, char* users_names_list);

int num_of_groups(GroupsManager* groups_manager);

int is_a_private(GroupsManager* groups_manager, char* group_name);

HashMap* group_users(GroupsManager* groups_manager, char* group_name);


#endif // GROUPS_MANAGER_H