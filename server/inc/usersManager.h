#ifndef __USERS_MANAGER_H__
#define __USERS_MANAGER_H__
#include "protocol.h"
#include "list.h"

typedef struct UserMng UserMng;

typedef enum UserMngResult
{
	SUCCESS,
	NOT_INITIAL_FAIL,
	MALLOC_FAIL,
	DUPLICATE_USERNAME,
	WRONG_DETAILS,
	OPEN_FILE_FAIL,
	NO_FOUND_IN_HASH,
/***/	CONNECT_TO_SAME_GROUP/***/
	
} UserMngResult;


UserMng* CreateRegisteredUsersPull (int _capacity);

void DestroyRegisteredUsersPull (UserMng* _map);

UserMngResult CreateUser (FirstAndSecond* _namePsw, UserMng* _userMng, int _isLowd);

UserMngResult UserJoinGroup (UserMng* _userMng, char* _username, char* _groupName);

UserMngResult UserLeaveGroup (UserMng* _userMng, char* _username, char* _groupName);

UserMngResult IsUsernameCorrect (UserMng* _map, char* _name);	

List* UserGetOutFromAllGroups (UserMng* _manager, char _name[]);

UserMngResult UserLogOut (UserMng* _manager, char _name[]);

int LogInUser (FirstAndSecond* _namePsw, UserMng* _userMng);

#endif
