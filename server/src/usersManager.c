#include "usersManager.h"
#include "userStruct.h"
#include "hash.map.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#define MAGIC_NUMBER 144522
#define SIZE 30
#define FILE_NAME "usersDetails.txt"
#define FROME_LOWD 1
#define NOT_FROME_LOWD 0

static void printKey(void* _word);
static void printValue(void* _value);

struct UserMng
{
	HashMap* m_dataStructure;
	int m_magicNumber;
};

size_t ConvertNameForHash (void* _key)
{
	int i;
	size_t multi = 1;
	char letter;
	int len = strlen ((char*)_key);
	for (i=0; i< len; i++)
	{
		letter = ((char*)_key)[i];
		multi *= letter;
	}
	return multi;
}

int CompareUserNames (void* _firstKey, void* _secondKey)	
{
	if (strcmp ((char*)_firstKey, (char*)_secondKey) == 0)
	{
		return 1;
	}
	return 0;
}


int LoadUsersFromFile (UserMng* _manager)
{
FILE* file;
char name[SIZE];
char password[SIZE];
FirstAndSecond* client;

	if(_manager == NULL)
	{
		return NOT_INITIAL_FAIL;
	}

	if((file = fopen(FILE_NAME, "r")) == NULL)
	{
		return OPEN_FILE_FAIL;
	}
	
	if(!feof(file))
	{
		client = (FirstAndSecond*) malloc (sizeof (FirstAndSecond));	
		fscanf(file,"%s\n %s\n", name, password);
		strcpy(client -> m_first, name);
		strcpy(client -> m_second, password);
		
		CreateUser (client, _manager, FROME_LOWD);
	}
		
			
	while (!feof(file))
	{
		client = (FirstAndSecond*) malloc (sizeof (FirstAndSecond));	
		fscanf(file,"%s\n %s\n", name, password);
		strcpy(client -> m_first, name);
		strcpy(client -> m_second, password);
		CreateUser (client, _manager, FROME_LOWD);		
	}
	
	
	
	fclose(file);
	
return SUCCESS;
}


UserMng* CreateRegisteredUsersPull (int _capacity)
{
	UserMng* manager;
	HashMap* map;
	manager = (UserMng*) malloc (sizeof (UserMng));
	if (manager == NULL)
	{
		return NULL;
	}
	map = HashMapCreate(_capacity, ConvertNameForHash, CompareUserNames);
	if (map == NULL)
	{
		return NULL; 
	}
	manager->m_dataStructure = map;
	LoadUsersFromFile (manager);
	return manager;
}

void DestroyOneUser (void* _user) /*maybe just change the DestroyUser function in userStruct to get void*/
{
	DestroyUser ((User*) _user);
}

void DestroyRegisteredUsersPull (UserMng* _userMng)
{
	HashMap* map;
	if (_userMng == NULL || _userMng->m_magicNumber != MAGIC_NUMBER)
	{
		return;
	}
	map = (HashMap*)_userMng->m_dataStructure;
	HashMapDestroy(&map , NULL , DestroyOneUser); /*Dont sure about the null*/
	_userMng->m_magicNumber = 0;
	free (_userMng);
}


UserMngResult InsertUserToFile(FirstAndSecond* _namePswr)
{
FILE* file; 
int i;

	if((file = fopen(FILE_NAME, "a")) == NULL)
	{
		printf ("open file failed\n");
		return OPEN_FILE_FAIL;
	}
			
	fprintf(file, "%s\n%s\n", _namePswr -> m_first,  _namePswr -> m_second);		
	
	fclose(file);
	
return SUCCESS;
}


UserMngResult CreateUser (FirstAndSecond* _namePsw, UserMng* _userMng, int _isLowd)
{
	User* user;
	int status;
	void* pValue;
	MapResult result;
	if (_namePsw == NULL || _userMng == NULL)
	{
		printf ("mallc fail 1\n");
		return MALLOC_FAIL;
	}
	result = HashMapFind(_userMng-> m_dataStructure , _namePsw->m_first , &pValue);
	if (result == MAP_KEY_NOT_FOUND_ERROR)
	{
		char* name = (char*) malloc (sizeof(_namePsw->m_first));
		if (name == NULL)
		{
		printf ("mallc fail 2\n");
			return MALLOC_FAIL;
		}
		strcpy (name, _namePsw->m_first);
		user = CreateNewUser ( _namePsw);
		if (user == NULL)
		{
		printf ("mallc fail 3\n");
			free (name);
			return MALLOC_FAIL;
		}
		status = HashMapInsert(_userMng -> m_dataStructure, name, user);
		if (status != MAP_SUCCESS)
		{
		printf ("mallc fail 4\n");
			free (name);
			DestroyUser (user);
			return MALLOC_FAIL;
		}
				
		if(_isLowd == NOT_FROME_LOWD)
		{			
			 InsertUserToFile( _namePsw);
				 
		}
		return SUCCESS;

	}
	return DUPLICATE_USERNAME;
}


static void printKey(void* _word)
{
	
	printf("Key: %s\n", ((char*)_word));
}



static void printValue(void* _value)
{
printf("name: %s\n", ((( User*)_value) -> m_username));
printf("pass: %s\n", ((( User*)_value) -> m_password));
printf("active: %d\n", ((( User*)_value) -> m_active));
printf("\n");		
}

int LogInUser (FirstAndSecond* _namePsw, UserMng* _userMng)
{
	MapResult result;
	void* pValue;
	char password[SIZE];
	if (_namePsw == NULL || _userMng == NULL || _userMng->m_dataStructure == NULL)
	{
		return NOT_INITIAL_FAIL;
	}
	result = HashMapFind(_userMng-> m_dataStructure , _namePsw->m_first , &pValue);
	if (result == MAP_SUCCESS)
	{
		GetUserPassword ((User*)pValue, password);
		if (strcmp (_namePsw->m_second ,password) == 0)
		{
			SetUserStatus ((User*) pValue, ACTIVE);
			return SUCCESS;
		}
		else
		{
			return WRONG_DETAILS;
		}
	}
	else
	{
		return WRONG_DETAILS;
	}
}

UserMngResult IsUsernameCorrect (UserMng* _userMng, char* _name)
{
	void* pValue;
	MapResult result = HashMapFind(_userMng-> m_dataStructure , _name , &pValue);
	if (result == MAP_SUCCESS)
	{
		return SUCCESS;
	}
	else
	{
		return NO_FOUND_IN_HASH;
	}
}

UserMngResult UserJoinGroup (UserMng* _userMng, char _username[], char _groupName[])
{
	MapResult result;
	void* pValue;
	if (_username == NULL || _groupName == NULL)
	{
		printf ("UserJoinGroup NOT_INITIAL_FAIL 1\n");
		return NOT_INITIAL_FAIL;
	}
	result = HashMapFind(_userMng-> m_dataStructure , _username , &pValue);
	if (result == MAP_SUCCESS)
	{
		result = AddGroupForUser ((User*)pValue, _groupName);
		if (result == USER_STRUCT_SUCCESS)
		{
			printf ("UserJoinGroup success\n");
			return SUCCESS;
		}
		else
		{
	/**/		return CONNECT_TO_SAME_GROUP;/***/
		}
		
	}
	else
	{
		printf ("UserJoinGroup NO_FOUND_IN_HASH\n");
		return NO_FOUND_IN_HASH;/*I dont sure it is neccsary*/
	}
}
	
	
UserMngResult UserLeaveGroup (UserMng* _userMng, char* _username, char* _groupName)
{
	MapResult result;
	void* pValue;
	if (_username == NULL || _groupName == NULL)
	{
		return NOT_INITIAL_FAIL;
	}
	result = HashMapFind(_userMng-> m_dataStructure , _username , &pValue);
	if (result == MAP_SUCCESS)
	{
	/**/	RemoveGroupFromUser ((User*)pValue, _groupName); /*there is easier way...*/
		return SUCCESS;/**/
	}
	else
	{
		return NO_FOUND_IN_HASH;
	}
}

List* UserGetOutFromAllGroups (UserMng* _manager, char _name[])
{
	MapResult result;
	void* pValue;
	result = HashMapFind(_manager-> m_dataStructure , _name , &pValue);
	if (result == MAP_SUCCESS)
	{
		printf("MAP_SUCCESS\n");
		return WhichGroups ((User*)pValue);
	}
}

UserMngResult UserLogOut (UserMng* _manager, char _name[])
{
	MapResult result;
	int status;
	void* pValue;
	result = HashMapFind(_manager-> m_dataStructure , _name , &pValue);
	if (result == MAP_SUCCESS)
	{
		UserNotActive ((User*) pValue);
		return SUCCESS;
	}
}		
/***/
/*UserMngResult UserLogOut (char _name[])
{
	result = HashMapFind(_userMng-> m_dataStructure , _username , &pValue);
	if (result == MAP_SUCCESS)
	{
		UserNotActive ((User*)pValue);
	}
	else
	{
		return NO_FOUND_IN_HASH;
	}
}*/
	
	
/***/	
	
