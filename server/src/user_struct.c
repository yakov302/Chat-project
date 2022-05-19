#include "user_struct.h"

#define MAGIC_NUMBER 144522
#define SIZE 30

User* CreateNewUser (FirstAndSecond* _namePsw)
{
	User* newUser = (User*) malloc (sizeof(User));
	if (newUser == NULL)
	{
		return NULL;
	}
	newUser->m_groups =  ListCreate();
	if (newUser->m_groups == NULL)
	{
		return NULL;
	}

	strcpy (newUser->m_username,_namePsw->m_first);
	strcpy (newUser->m_password, _namePsw->m_second);
	newUser->m_active = NO_ACTIVE;
	return newUser;
}

void DestroyUser (User* _user)
{
	if (_user == NULL || _user->m_magicNumber != MAGIC_NUMBER)
	{
		return;
	}

	_user->m_magicNumber = 0;
	free (_user);
}

void GetUserPassword (User* _user, char _password[])
{
	if (_user == NULL)
	{
		return;
	}
	strcpy (_password, _user -> m_password);
}

int SetUserStatus (User* _user, int _status)
{
	_user->m_active = _status;
}

int AddGroupForUser (User* _user, char* _groupName)
{

	char* groupName;
	ListItr begin, end;
	void* pValue;
 	begin = ListItrBegin(_user->m_groups);
	end = ListItrEnd(_user->m_groups);
	
	printf("AddGroupForUser - top\n");
	while (begin != end)
	{
		pValue = ListItrGet(begin);
		if (strcmp ((char*) pValue, _groupName) != 0)
		{
			begin = ListItrNext(begin);
		}
		else
		{
			return ALMOST_IN_THE_GROUP;
		}
	}
	groupName = malloc (sizeof (char) * SIZE);
	if (groupName == NULL)
	{
		return USER_STRUCT_MALLOC_FAIL;
	}
	strcpy (groupName, _groupName);
	ListPushTail(_user->m_groups, groupName);
	printf("ListPushTail - line 94\n");
	
	return USER_STRUCT_SUCCESS;
}
	
int RemoveGroupFromUser (User* _user, char* _groupName)
{
	ListItr begin, end;
	void* pValue;
 	begin = ListItrBegin(_user->m_groups);
	end = ListItrEnd(_user->m_groups);
	while (begin != end)
	{
		pValue = ListItrGet(begin);
		if (strcmp ((char*) pValue, _groupName) != 0)
		{
			begin = ListItrNext(begin);
		}
		else
		{
			ListItrRemove(begin);
			return USER_STRUCT_SUCCESS;
		}
	}
	return USER_STRUCT_NOT_FOUND_IN_HASH;
}
	
List* WhichGroups (User* _user)
{
	return _user->m_groups;
}

int UserNotActive (User* _user)
{
	ListItr itr, end;
	itr = ListItrBegin(_user ->m_groups);
	end  = ListItrEnd(_user ->m_groups);
	while (itr != end)
	{	
		ListItrRemove(itr);
		itr = ListItrNext(itr);
	}
	_user->m_active = NO_ACTIVE;
	return USER_STRUCT_SUCCESS;
}	

	
	
	
	
	
	
	

