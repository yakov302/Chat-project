#include "group.h"

Group* create_group(const char* group_name, char* group_ip)
{
    Group* group = (Group*)malloc(sizeof(Group));
	if(group == NULL)
	    return NULL;

	strcpy (group->m_name, group_name);
    strcpy (group->m_ip, group_ip);
	group->m_num_of_clients = 0;
	group->m_magic_number = MAGIC_NUMBER;
	return group;
}

void destroy_group(Group* group)
{
    if (group == NULL || group->m_magic_number != MAGIC_NUMBER)
	    return;
	
	group->m_magic_number = 0;
	free (group);
    group = NULL;
}

void group_ip(Group* group, char* group_ip)
{
    if(group == NULL || group_ip == NULL)
	    return;
	
	strcpy(group_ip, group->m_ip);
}

int number_of_clients(Group* group)
{
    if(group == NULL)
	    return 0;
	
    return group->m_num_of_clients;
}

void decrese_num_of_cleints (Group* group)
{
    if(group == NULL)
	    return;
	
    group->m_num_of_clients--; 
}

void increse_num_of_cleints (Group* group)
{
    if(group == NULL)
	    return;
	
    group->m_num_of_clients++; 
}
