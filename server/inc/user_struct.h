#ifndef __USER_STRUCT_H__
#define __USER_STRUCT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "protocol.h"
#include "gen.list.h"
#include "list.h"

#define ACTIVE 1
#define NO_ACTIVE 0
#define SIZE 30

typedef enum userStrResult
{
	USER_STRUCT_SUCCESS,
	USER_STRUCT_MALLOC_FAIL,
	USER_STRUCT_NOT_INITIALIZED,
	USER_STRUCT_NOT_FOUND_IN_HASH,
	ALMOST_IN_THE_GROUP
}userStrResult;

struct User
{
	char m_username[SIZE];
	char m_password[SIZE];
	List* m_groups;
	int m_active;
	int m_magicNumber;
};
typedef struct User User;

/**This function create User type struct, which contains name, password, list of groups and status active or no.
Input: FirstAndSecond* struct- contains two members - name and password.
Output: pointer to User struct. if allocation failed retturn NULL**/
User* CreateNewUser (FirstAndSecond* _namePsw);

/**This function destroy User struct and free all allocations.
Input: declare void* because of the typedef in list.h, used for User struct created above.
Output: void.**/
void DestroyUser (User* _user);

/**The function get User struct created above, and parameter to get into it user's password**/
void GetUserPassword (User* _user, char _password[]);

/**The function set user status to active / no active. 
Input: User struct created above, and status to set.
Output: void**/
int SetUserStatus (User* _user, int _status);

/**Function remove group from list of user's groups. 
Input: User struct created above, and group name to remove.
Output: USER_STRUCT_SUCCESS - if group removed successfully.
	USER_STRUCT_NOT_FOUND_IN_HASH - if group didn't found
	USER_STRUCT_NOT_INITIALIZED - if one of parameters is NULL**/
int RemoveGroupFromUser (User* _user, char* _groupName);

/**Function add group to list of user's groups. 
Input: User struct created above, and group name to add.
Output: USER_STRUCT_SUCCESS - if group removed successfully.
	ALMOST_IN_THE_GROUP - if user allmost connected to group.
	USER_STRUCT_MALLOC_FAIL - if allocation failed.
	USER_STRUCT_NOT_INITIALIZED- if one of parameters is NULL**/
int AddGroupForUser (User* _user, char* _groupName);

/**This function remove all groups and change user's status to no active.
Input: User struct created above, and group name to add.
Output: USER_STRUCT_NOT_INITIALIZED - if user is NULL, and USER_STRUCT_SUCCESS if success.**/
int UserNotActive (User* _user);

/**This function get User struct created above and return pointer to list which contains all user's pfroups.**/ 
List* WhichGroups (User* _user);


#endif
