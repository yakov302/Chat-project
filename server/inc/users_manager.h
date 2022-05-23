#ifndef __USERS_MANAGER_H__
#define __USERS_MANAGER_H__

#include <stdlib.h>
#include <string.h>

#include "user.h"
#include "hash.map.h"

#define FILE_NAME "usersDetails.txt"
#define MAGIC_NUMBER 144522
#define NOT_FROME_LOWD 0
#define FROME_LOWD 1
#define NOT_EQUAL 0
#define SIZE 30
#define EQUAL 1

typedef struct UserMng
{
	HashMap* m_dataStructure;
	int m_magicNumber;
}UserMng;

typedef enum UserMngResult
{
	SUCCESS,
	NOT_INITIAL_FAIL,
	MALLOC_FAIL,
	DUPLICATE_USERNAME,
	WRONG_DETAILS,
	OPEN_FILE_FAIL,
	NO_FOUND_IN_HASH,
	CONNECT_TO_SAME_GROUP
} UserMngResult;

/**This function create a struct that hold data structure with users details.
Input: capacity of data structure.
OutPut: pointer to UserMng struct.
Errors: If allocation fail return NULL**/
UserMng* CreateUserManager(int _capacity);

/**This function destroy the struct and the data structure tht hold all users.
Input: pointer to UserMng struct.
Output: void. **/
void DestroyUserManager(UserMng* _map);

/**This function create a struct for every user that registrated to application, and return status.
Input: FirstAndSecond* struct - contains two members, in this case username & password.
	UserMng* struct created above,
	and a flag to sign if the function called by a new user registration or by loading details from file.
Output & errors : MALLOC_FAIL - if allocation failed.
		DUPLICATE_USERNAME - if user registrated with name allready used.
		SUCCESS - if create successfully. 
		NOT_INITIAL_FAIL - if one of parameters is NULL **/
UserMngResult CreateUser(FirstAndSecond* _namePsw, UserMng* _userMng, int _isLowd);

/**This function log in registrated user, and return status.
Input: FirstAndSecond* struct - contains two members, in this case username & password.
	UserMng* struct created above.
Output: WRONG_DETAILS - if name or password doesn't correct.
	NOT_INITIAL_FAIL - if one of parameters is NULL.
	SUCCESS - user loged in successfully.**/
int LogInUser(FirstAndSecond* _namePsw, UserMng* _userMng);

/**This function Join User to group, and add name of group to details of user, and return status.
Input: UserMng* struct created above, username that asked to join the group, and name of the group.
Output: CONNECT_TO_SAME_GROUP - if username allready connected to group.
	NOT_INITIAL_FAIL - if one of parameters is NULL
	NO_FOUND_IN_HASH - if userName didn't found.
	SUCCESS - group added successfully.**/
UserMngResult UserJoinGroup(UserMng* _userMng, char* _username, char* _groupName);

/**This function remove a group from user details, and return status.
Input: UserMng* struct created above, username that asked to join the group, and name of the group.
Output: NOT_INITIAL_FAIL - if one of parameters is NULL
	NO_FOUND_IN_HASH - if userName didn't found.
	SUCCESS - group removed successfully.**/
UserMngResult UserLeaveGroup(UserMng* _userMng, char* _username, char* _groupName);

/**This function remove all user's groups, and change his status to no active. return status.
Input: UserMng* struct created above, and username.
Output: SUCCESS - if loged out successfully
	NO_FOUND_IN_HASH - if userName didn't found.
	NOT_INITIAL_FAIL - if one of parameters is NULL**/
UserMngResult UserLogOut(UserMng* _manager, char _name[]);

/**This function return user struct by yuser name.
Input: UserMng* struct created above, and username.
Output: pointer to user struct.**/
User* GiveUserByName(UserMng* _manager, char _name[]);


#endif //__USERS_MANAGER_H__
