#include "group_struct.h"

#define MAGIC_NUMBER 144522
#define SIZE 30

struct Group
{
	char m_groupName[SIZE];
	char m_groupIp[SIZE];
	int m_numOfClients;
	int m_magicNumber;
};

Group* CreateNewGroup (char _groupName[])
{	
Group* newGroup = (Group*) malloc (sizeof(Group));

	if (newGroup == NULL)
	{return NULL;}

	strcpy (newGroup -> m_groupName, _groupName);
	newGroup -> m_numOfClients = 0;
	newGroup -> m_magicNumber = MAGIC_NUMBER;

return newGroup;
}

void DestroyGroup (Group* _group)
{
	if (_group == NULL || _group -> m_magicNumber != MAGIC_NUMBER)
	{
		return;
	}
	
	_group -> m_magicNumber = 0;
	free (_group);
}

void GetGroupIp (Group* _group, char _groupIp[])
{
	if (_group == NULL)
	{return;}
	
	strcpy (_groupIp, _group -> m_groupIp);
}

int GetGroupNumOfClients (Group* _group)
{
	if (_group == NULL)
	{return 0;}
	
return  _group -> m_numOfClients;
}

void SetGroupIp (Group* _group, char _groupIp[])
{
	if (_group == NULL)
	{return;}
	
	strcpy(_group -> m_groupIp, _groupIp);
}

void DecreseNumOfCleints (Group* _pValue)
{
	_pValue->m_numOfClients--;
}

void IncreseNumOfCleints (Group* _pValue)
{
	_pValue->m_numOfClients++;
}

int GetNumOfClients(Group* _pValue)
{
	return _pValue->m_numOfClients;
}



