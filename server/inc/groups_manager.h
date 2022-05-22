#ifndef __GROUPS_MANAGER_H__
#define __GROUPS_MANAGER_H__

#include <stdlib.h>
#include <string.h>

#include "group.h"
#include "hash.map.h"
#include "queue.h"

#define IP_SIZE 20

typedef struct GrupsMng GrupsMng;

typedef enum GrupsMngResult
{
    MANAGER_NOT_INITIALIZED,
    GROUP_NOT_INITIALIZED,
    GROUP_MALLOC_FAIL,
    HASH_MAP_INSERT_FAIL,
    DUPLICATE_GROUP_NAME_FAIL,
    GROUP_NOT_FOUND_IN_HASH,
    GROUP_DELETE,
    GROUP_SUCCESS,
    NO_EXISTING_GROUPS_GM,
    PRINT_EXISTING_GROUPS_SUCCESS_GM,
    BUFFER_NOT_INITIALIZED
}GrupsMngResult;


/*Description:
Create groups manager.

Input:
 _capacity - The size of the HashMap that will hold all the groups .

Output:
NULL - If memory allocation failed, if HashMapCreate fail or if QueueCreate.
manager - Pointer to the  group manager struct.*/
GrupsMng* CreateGroupsManager (int _capacity);

/*Description:
Destroy the groups manager.

Input:
_manager - Pointer to the group manager struct.*/
void DestroyGroupsManager (GrupsMng* _manager);

/*Description:
Creates new group and inserts it into the data structures.

Input:
_manager - Pointer to the group manager struct.
_groupName[] - group name;
_buffer[] - Gets the group IP.

Output:
MANAGER_NOT_INITIALIZED - If _manager == NULL.
GROUP_NOT_INITIALIZED - if memory allocation failed.
HASH_MAP_INSERT_FAIL - If the group failed to enter the database.
DUPLICATE_GROUP_NAME_FAIL - If there is already a group with this name. (The group does not open).
GROUP_SUCCESS - If the group was created successfully and entered into the database.*/
GrupsMngResult CreateGroup (char _groupName[] , GrupsMng* _manager, char _buffer[]);

/*Description:
Increases the number of clients associated with the group by 1 and returns to the buffer the group IP.

Input:
_manager - Pointer to the group manager struct.
_groupName[] - group name;
_buffer[] - Gets the group IP.

Output:
MANAGER_NOT_INITIALIZED - If _manager == NULL.
GROUP_NOT_FOUND_IN_HASH - If the group name does not exist.
GROUP_SUCCESS - If the number of clients associated with the group increased by 1 and the IP returned to the buffer.*/
GrupsMngResult JoinExistingGroup (char _groupName[] , GrupsMng* _manager, char _buffer[]);

/*Description:
Decreases the number of clients associated with the group by 1.

Input:
_manager - Pointer to the group manager struct.
_groupName[] - group name;

Output:
MANAGER_NOT_INITIALIZED - If _manager == NULL.
GROUP_NOT_FOUND_IN_HASH - If the group name does not exist.
GROUP_DELETE - If there are no clients in the group it is deleted.
GROUP_SUCCESS - If the number of clients associated with the group decreases by 1.*/
GrupsMngResult LeaveGroup (char _groupName[] , GrupsMng* _manager, char _buffer[]);

/*Description:
Calls HashMap function that put in _str[] all the groups stored in the HashMap.

Input:
 _groups - Pointer to the group manager struct.
_str[] - wiil accept all the groups in HashMap.*/
void giveGroups(GrupsMng* _groups, char _str[]);


#endif
