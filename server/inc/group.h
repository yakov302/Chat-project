#ifndef __GROUP_STRUCT_H__
#define __GROUP_STRUCT_H__

#include <stdlib.h>
#include <string.h>

#define MAGIC_NUMBER 144522
#define SIZE 30

typedef struct Group
{
	char m_groupName[SIZE];
	char m_groupIp[SIZE];
	int m_numOfClients;
	int m_magicNumber;
}Group;

/*Description:
Creates group struct and updates its members.

Input:
_groupName[] - Group name

Output:
NULL - If memory allocation failed.
newGroup - Pointer to the group struct. */
Group* CreateNewGroup (char _groupName[]);

/*Description:
Destroy the group.

Input:
*_group - Pointer to the group struct.*/
void DestroyGroup (Group* _group);

/*Description:
Put inside the variable _groupIp[] the IP of the group.

Input:
*_group - Pointer to the group struct.
_groupIp[] - the variable that will get the IP of the group.*/
void GetGroupIp (Group* _group, char _groupIp[]);

/*Description:
Returns the number of clients in the group.

Input:
*_group - Pointer to the group struct.

Output:
_group -> m_numOfClients - the number of clients in the group*/
int GetGroupNumOfClients (Group* _group);

/*Description:
Assigns group IP.

Input:
*_group - Pointer to the group struct.
_groupIp[] - The IP that will link to the group.*/
void SetGroupIp (Group* _group, char _groupIp[]);

/*Description:
Decreases by 1 the number of clients in the group.

Input:
*_group - Pointer to the group struct.*/
void DecreseNumOfCleints (Group* _pValue);

/*Description:
Increases by 1 the number of clients in the group.

Input:
*_group - Pointer to the group struct.*/
void IncreseNumOfCleints (Group* _pValue);


#endif //__GROUP_STRUCT_H__
